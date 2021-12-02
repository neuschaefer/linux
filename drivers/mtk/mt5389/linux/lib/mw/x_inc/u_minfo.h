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
/*! @file u_minfo.h 
 *  $RCSfile: $
 *  $Revision: #2 $
 *  $Date: 2012/07/25 $
 *  $Author: jiangbo.wang $
 *  
 *  @par Description:
 *         This file exports all media info specific definitions and structures.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_MUTIL_MINFO
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_MINFO_H_
#define _U_MINFO_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__
    
#include "u_common.h"
#include "u_handle.h"
#include "u_mfmtrecg.h"
#include "u_drm.h"
#include "u_divx_drm.h"
#include "u_drm_drv.h"
    
#else
    
#include "u_common.h"
#include "u_handle.h"
#include "u_mfmtrecg.h"
#include "u_drm.h"
#include "u_divx_drm.h"
#include "u_drm_drv.h"
    
#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define AVI_MEDIA_INFO_NAME "avi minfo"    /**<        */
#define WAVE_MEDIA_INFO_NAME "wave minfo" /*add for wave support*/    /**<        */
#define MP3_MEDIA_INFO_NAME "mp3 minfo"    /**<        */
#define MP2_MEDIA_INFO_NAME "mp2 minfo"    /**<        */
#define ASF_MEDIA_INFO_NAME "asf minfo"    /**<        */
#define WMA_MEDIA_INFO_NAME "wma minfo"    /**<        */
#define MP4_MEDIA_INFO_NAME "mp4 minfo"    /**<        */
#define MKV_MEDIA_INFO_NAME "mkv minfo"    /**<        */
#define PS_VOB_MEDIA_INFO_NAME "ps_vob minfo"    /**<        */
#define PS_MPG_MEDIA_INFO_NAME "ps_mpg minfo"    /**<        */
#define PS_DAT_MEDIA_INFO_NAME "ps_dat minfo"    /**<        */
#define FLV_MEDIA_INFO_NAME "flv minfo"  /*add for flv*/
#define OGG_MEDIA_INFO_NAME "ogg_minfo"    /**<add for ogg*/

#define TITLE_NAME_MAX      (UINT32)0x80
#define STRING_LEN_MAX      (UINT32)0x80
#define MAX_NS_MP4_AUD  3
#define MAX_NS_MP4_VID  1
#define MAX_NS_MP4_SBTL 4


/* This structure enumerates all types of the notification of MINFO to AP. */
/*------------------------------------------------------------------*/
/*! @enum  MINFO_NFY_EVENT_TYPE_T
 *  @brief Minfo notify event type.
 *  @code
 *  typedef enum
 *  {
 *      MINFO_NFY_EVENT_PARSING_DONE,
 *      MINFO_NFY_EVENT_HANDLE_CLOSED,
 *      MINFO_NFY_EVENT_ERROR
 *  }   MINFO_NFY_EVENT_TYPE_T;
 *  @endcode
 *  @li@c  MINFO_NFY_EVENT_PARSING_DONE                      - Parsing done.
 *  @li@c  MINFO_NFY_EVENT_HANDLE_CLOSED                     - Handle closed.
 *  @li@c  MINFO_NFY_EVENT_ERROR                             - Error.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MINFO_NFY_EVENT_PARSING_DONE,
    MINFO_NFY_EVENT_HANDLE_CLOSED,
    MINFO_NFY_EVENT_ERROR
    /*MINFO_NFY_EVENT_OUT_OF_SPEC-------not used now*/
}   MINFO_NFY_EVENT_TYPE_T;


/* This structure enumerates all info type of media info.  */
/*------------------------------------------------------------------*/
/*! @enum MINFO_INFO_TYPE_T
 *  @brief Infor type.
 *  @code
 *  typedef enum
 *  {
 *      MINFO_INFO_TYPE_UNKNOWN = 0,
 *      MINFO_INFO_TYPE_GENERIC,
 *      MINFO_INFO_TYPE_META_DATA,
 *      MINFO_INFO_TYPE_ID3_DATA,
 *      MINFO_INFO_TYPE_VIDEO_ATTR,
 *      MINFO_INFO_TYPE_AUDIO_ATTR,
 *      MINFO_INFO_TYPE_SP_ATTR,
 *      MINFO_INFO_TYPE_IDX_INFO,
 *      MINFO_INFO_TYPE_DRM_INFO,
 *      MINFO_INFO_TYPE_FONT_INFO,  
 *      MINFO_INFO_TYPE_FONT_DATA
 *  #if SUPPORT_MULTI_INFO
 *      ,
 *      MINFO_INFO_TYPE_SCALE_INFO
 *  #endif
 *  } MINFO_INFO_TYPE_T;
 *  @endcode
 *  @li@c  MINFO_INFO_TYPE_UNKNOWN                           - Unknown.
 *  @li@c  MINFO_INFO_TYPE_GENERIC                           - Generic
 *  @li@c  MINFO_INFO_TYPE_META_DATA                         - Meta data.
 *  @li@c  MINFO_INFO_TYPE_ID3_DATA                          - ID3 data.
 *  @li@c  MINFO_INFO_TYPE_VIDEO_ATTR                        - Video attribute.
 *  @li@c  MINFO_INFO_TYPE_AUDIO_ATTR                        - Audio attribute.
 *  @li@c  MINFO_INFO_TYPE_SP_ATTR                           - Subtitle infor.
 *  @li@c  MINFO_INFO_TYPE_IDX_INFO                          - Idx infor.
 *  @li@c  MINFO_INFO_TYPE_DRM_INFO                          - Drm infor.
 *  @li@c  MINFO_INFO_TYPE_FONT_INFO                         - Font infor.
 *  @li@c  MINFO_INFO_TYPE_FONT_DATA                         - Font data.
 *  @li@c  MINFO_INFO_TYPE_SCALE_INFO                        - Scale infor.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MINFO_INFO_TYPE_UNKNOWN = 0,
    MINFO_INFO_TYPE_GENERIC,
    MINFO_INFO_TYPE_META_DATA,
    MINFO_INFO_TYPE_ID3_DATA,
    MINFO_INFO_TYPE_ID3_RAW_DATA,
    MINFO_INFO_TYPE_VIDEO_ATTR,
    MINFO_INFO_TYPE_AUDIO_ATTR,
    MINFO_INFO_TYPE_SP_ATTR,
    MINFO_INFO_TYPE_IDX_INFO,
    MINFO_INFO_TYPE_DRM_INFO,
    MINFO_INFO_TYPE_FONT_INFO,  /*add for font info*/
    MINFO_INFO_TYPE_FONT_DATA,
    MINFO_INFO_TYPE_RAW_DATA,
    MINFO_INFO_TYPE_VIDEO_ENCRYPTION_INFO,
    MINFO_INFO_TYPE_AUDIO_ENCRYPTION_INFO
#if SUPPORT_MULTI_INFO
    ,MINFO_INFO_TYPE_SCALE_INFO
#endif
#if 1/*MKV DivXPlus*/
    ,MINFO_INFO_TYPE_TITLE_NUM
    ,MINFO_INFO_TYPE_TITLE_INFO
    ,MINFO_INFO_TYPE_PLAYLIST_INFO
    ,MINFO_INFO_TYPE_CHAP_INFO
    ,MINFO_INFO_TYPE_DISP_INFO    
    ,MINFO_INFO_TYPE_SET_TITLE
    ,MINFO_INFO_TYPE_CODEC
    ,MINFO_INFO_TYPE_SET_LANGUAGE
#endif
} MINFO_INFO_TYPE_T;

typedef enum
{
    MINFO_INFO_CODEC_VID_ENC_UNKONWN =0,
    MINFO_INFO_CODEC_VID_ENC_DIVX
}MINFO_INFO_CODEC_ENC_T;

typedef UINT16 MINFO_REC_IDX_T;/*x_minfo_get_info */    /**<        */
#define MINFO_REC_FULL_IDX        ((MINFO_REC_IDX_T) 0) /* all records of specific type*/    /**<        */

/*define variable type of stream id */
typedef UINT32 MINFO_AVI_STM_ID_T;    /**<        */
typedef UINT32 MINFO_ASF_STM_ID_T;    /**<        */
typedef UINT32 MINFO_MP4_STM_ID_T;    /**<        */
typedef UINT32 MINFO_MKV_STM_ID_T;    /**<        */
typedef UINT32 MINFO_RM_STM_ID_T;    /**<        */
typedef UINT32 MINFO_AIF_STM_ID_T;    /**<        */
typedef UINT32 MINFO_FLV_STM_ID_T;
typedef UINT32 MINFO_CMPB_STM_ID_T;
typedef UINT32 MINFO_OGG_STM_ID_T;

/*------------------------------------------------------------------*/
/*! @struct MINFO_MPEG_2_STREAM_ID
 *  @brief
 *  @code
 *  typedef struct _MINFO_MPEG_2_STREAM_ID
 *  {
 *       UINT16 ui1_stm_id;
 *       UINT16 ui1_sub_stm_id;
 *  }MINFO_MPEG_2_STREAM_ID;
 *  @endcode
 *  @li@c  ui1_stm_id                                        - Stream id
 *  @li@c  ui1_sub_stm_id                                    - Sub stream id.
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_MPEG_2_STREAM_ID
{
     UINT16 ui1_stm_id;
     UINT16 ui1_sub_stm_id;
}MINFO_MPEG_2_STREAM_ID;



typedef UINT8   MINFO_INFO_AUD_LAYER_T;    /**<        */

/* This structure enumerates all info type of media info.  */
#if 0
/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      MINFO_INFO_TYPE_MEDIA_SIZE_IN_BYTES = 0,
 *      MINFO_INFO_TYPE_ID3_ATTACHED_PIC_BITMASK, 
 *      MINFO_INFO_TYPE_ID3_ATTACHED_PIC, 
 *      MINFO_INFO_TYPE_ID3_YEAR,
 *      MINFO_INFO_TYPE_ID3_TITLE,
 *      MINFO_INFO_TYPE_ID3_ARTIST,
 *      MINFO_INFO_TYPE_ID3_ALBUM,
 *      MINFO_INFO_TYPE_ID3_GENRE,
 *      MINFO_INFO_TYPE_ID3_V1_LENG,
 *      MINFO_INFO_TYPE_ID3_V2_LENG
 *  } MINFO_INFO_TYPE_T;
 *  @endcode
 *  @li@c  MINFO_INFO_TYPE_MEDIA_SIZE_IN_BYTES               -
 *  @li@c  MINFO_INFO_TYPE_ID3_ATTACHED_PIC_BITMASK          -
 *  @li@c  MINFO_INFO_TYPE_ID3_ATTACHED_PIC                  -
 *  @li@c  MINFO_INFO_TYPE_ID3_YEAR                          -
 *  @li@c  MINFO_INFO_TYPE_ID3_TITLE                         -
 *  @li@c  MINFO_INFO_TYPE_ID3_ARTIST                        -
 *  @li@c  MINFO_INFO_TYPE_ID3_ALBUM                         -
 *  @li@c  MINFO_INFO_TYPE_ID3_GENRE                         -
 *  @li@c  MINFO_INFO_TYPE_ID3_V1_LENG                       -
 *  @li@c  MINFO_INFO_TYPE_ID3_V2_LENG                       -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MINFO_INFO_TYPE_MEDIA_SIZE_IN_BYTES = 0,
    MINFO_INFO_TYPE_ID3_ATTACHED_PIC_BITMASK, /* UINT32 */
    MINFO_INFO_TYPE_ID3_ATTACHED_PIC, /* *MINFO_INFO_PIC_T */
    MINFO_INFO_TYPE_ID3_YEAR,
    MINFO_INFO_TYPE_ID3_TITLE,
    MINFO_INFO_TYPE_ID3_ARTIST,
    MINFO_INFO_TYPE_ID3_ALBUM,
    MINFO_INFO_TYPE_ID3_GENRE,
    MINFO_INFO_TYPE_ID3_V1_LENG,
    MINFO_INFO_TYPE_ID3_V2_LENG
} MINFO_INFO_TYPE_T;
#endif

/* This structure enumerates all encoding types of audio stream.*/
/*------------------------------------------------------------------*/
/*! @enum MINFO_INFO_AUDIO_ENC_T
 *  @brief Audio codec type.
 *  @code
 *  typedef enum
 *  {
 *      MINFO_INFO_AUD_ENC_UNKNOWN =0,
 *      MINFO_INFO_AUD_ENC_AC3,
 *      MINFO_INFO_AUD_ENC_MPEG_1,
 *      MINFO_INFO_AUD_ENC_MPEG_2,
 *      MINFO_INFO_AUD_ENC_PCM,
 *      MINFO_INFO_AUD_ENC_ADPCM,
 *      MINFO_INFO_AUD_ENC_TV_SYS,
 *      MINFO_INFO_AUD_ENC_DTS,
 *      MINFO_INFO_AUD_ENC_AAC,
 *      MINFO_INFO_AUD_ENC_EU_CANAL_PLUS,
 *      MINFO_INFO_AUD_ENC_WMA_V1,
 *      MINFO_INFO_AUD_ENC_WMA_V2,
 *      MINFO_INFO_AUD_ENC_WMA_V3,
 *      MINFO_INFO_AUD_ENC_WMA_PRO,
 *      MINFO_INFO_AUD_ENC_WMA_LOSSLESS,
 *      MINFO_INFO_AUD_ENC_LPCM,
 *      MINFO_INFO_AUD_ENC_COOK
 *  }MINFO_INFO_AUDIO_ENC_T;
 *  @endcode
 *  @li@c  MINFO_INFO_AUD_ENC_UNKNOWN                        - Unknown.
 *  @li@c  MINFO_INFO_AUD_ENC_AC3                            - AC3
 *  @li@c  MINFO_INFO_AUD_ENC_MPEG_1                         - Mpeg 1.
 *  @li@c  MINFO_INFO_AUD_ENC_MPEG_2                         - Mpeg 2.
 *  @li@c  MINFO_INFO_AUD_ENC_PCM                            - Pcm.
 *  @li@c  MINFO_INFO_AUD_ENC_ADPCM                          - Adpcm.
 *  @li@c  MINFO_INFO_AUD_ENC_TV_SYS                         - TV sys.
 *  @li@c  MINFO_INFO_AUD_ENC_DTS                            - Dts.
 *  @li@c  MINFO_INFO_AUD_ENC_AAC                            - Aac
 *  @li@c  MINFO_INFO_AUD_ENC_EU_CANAL_PLUS                  - Eu canal plus.
 *  @li@c  MINFO_INFO_AUD_ENC_WMA_V1                         - Wma v1.
 *  @li@c  MINFO_INFO_AUD_ENC_WMA_V2                         - Wma v2.
 *  @li@c  MINFO_INFO_AUD_ENC_WMA_V3                         - Wma v3.
 *  @li@c  MINFO_INFO_AUD_ENC_WMA_PRO                        - Wma Pro
 *  @li@c  MINFO_INFO_AUD_ENC_WMA_LOSSLESS                   - Wma Lossless
 *  @li@c  MINFO_INFO_AUD_ENC_LPCM                           - Lpcm.
 *  @li@c  MINFO_INFO_AUD_ENC_COOK                           - Cook.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MINFO_INFO_AUD_ENC_UNKNOWN =0,
    MINFO_INFO_AUD_ENC_AC3,
    MINFO_INFO_AUD_ENC_MPEG_1,
    MINFO_INFO_AUD_ENC_MPEG_2,
    MINFO_INFO_AUD_ENC_PCM,
    MINFO_INFO_AUD_ENC_ADPCM,
    MINFO_INFO_AUD_ENC_A_PCM,
    MINFO_INFO_AUD_ENC_U_PCM,
    MINFO_INFO_AUD_ENC_DVI_ADPCM,
    MINFO_INFO_AUD_ENC_TV_SYS,
    MINFO_INFO_AUD_ENC_DTS,
    MINFO_INFO_AUD_ENC_AAC,
    MINFO_INFO_AUD_ENC_EU_CANAL_PLUS,
    MINFO_INFO_AUD_ENC_WMA_V1,
    MINFO_INFO_AUD_ENC_WMA_V2,
    MINFO_INFO_AUD_ENC_WMA_V3,
    MINFO_INFO_AUD_ENC_WMA_PRO,
    MINFO_INFO_AUD_ENC_WMA_LOSSLESS,
    MINFO_INFO_AUD_ENC_LPCM,
    MINFO_INFO_AUD_ENC_COOK,
    MINFO_INFO_AUD_ENC_VORBIS,
    MINFO_INFO_AUD_ENC_AMR,
    MINFO_INFO_AUD_ENC_AWB,
    MINFO_INFO_AUD_ENC_EC3
}MINFO_INFO_AUDIO_ENC_T;

/*------------------------------------------------------------------*/
/*! @enum MINFO_INFO_AUD_PCM_ALIGN
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      MINFO_INFO_AUD_PCM_LITTLE_ENDIAN,
 *      MINFO_INFO_AUD_PCM_BIG_ENDIAN
 *  }MINFO_INFO_AUD_PCM_ALIGN;
 *  @endcode
 *  @li@c  MINFO_INFO_AUD_PCM_LITTLE_ENDIAN                  - 
 *  @li@c  MINFO_INFO_AUD_PCM_BIG_ENDIAN                     -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MINFO_INFO_AUD_PCM_LITTLE_ENDIAN,
    MINFO_INFO_AUD_PCM_BIG_ENDIAN
}MINFO_INFO_AUD_PCM_ALIGN;

/* This structure enumerates all encoding types of video stream.*/
/*------------------------------------------------------------------*/
/*! @enum MINFO_INFO_VIDEO_ENC_T
 *  @brief Video codec type.
 *  @code
 *  typedef enum
 *  {
 *      MINFO_INFO_VID_ENC_UNKNOWN =0,
 *      MINFO_INFO_VID_ENC_MPEG_1,
 *      MINFO_INFO_VID_ENC_MPEG_2,
 *      MINFO_INFO_VID_ENC_MPEG_4,
 *      MINFO_INFO_VID_ENC_DIVX_311,
 *      MINFO_INFO_VID_ENC_DIVX_4,
 *      MINFO_INFO_VID_ENC_DIVX_5,
 *      MINFO_INFO_VID_ENC_XVID,
 *      MINFO_INFO_VID_ENC_WMV1,
 *      MINFO_INFO_VID_ENC_WMV2,
 *      MINFO_INFO_VID_ENC_WMV3,
 *      MINFO_INFO_VID_ENC_WVC1,
 *      MINFO_INFO_VID_ENC_H264,
 *      MINFO_INFO_VID_ENC_H263,
 *      MINFO_INFO_VID_ENC_MJPG,
 *      MINFO_INFO_VID_ENC_RV8,
 *      MINFO_INFO_VID_ENC_RV9
 *  }MINFO_INFO_VIDEO_ENC_T;
 *  @endcode
 *  @li@c  MINFO_INFO_VID_ENC_UNKNOWN                        -
 *  @li@c  MINFO_INFO_VID_ENC_MPEG_1                         -
 *  @li@c  MINFO_INFO_VID_ENC_MPEG_2                         -
 *  @li@c  MINFO_INFO_VID_ENC_MPEG_4                         -
 *  @li@c  MINFO_INFO_VID_ENC_DIVX_311                       -
 *  @li@c  MINFO_INFO_VID_ENC_DIVX_4                         -
 *  @li@c  MINFO_INFO_VID_ENC_DIVX_5                         -
 *  @li@c  MINFO_INFO_VID_ENC_XVID                           -
 *  @li@c  MINFO_INFO_VID_ENC_WMV1                           -
 *  @li@c  MINFO_INFO_VID_ENC_WMV2                           -
 *  @li@c  MINFO_INFO_VID_ENC_WMV3                           -
 *  @li@c  MINFO_INFO_VID_ENC_WVC1                           -
 *  @li@c  MINFO_INFO_VID_ENC_H264                           -
 *  @li@c  MINFO_INFO_VID_ENC_H263                           -
 *  @li@c  MINFO_INFO_VID_ENC_MJPG                           -
 *  @li@c  MINFO_INFO_VID_ENC_RV8                            -
 *  @li@c  MINFO_INFO_VID_ENC_RV9                            -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MINFO_INFO_VID_ENC_UNKNOWN =0,
    MINFO_INFO_VID_ENC_MPEG_1,
    MINFO_INFO_VID_ENC_MPEG_2,
    MINFO_INFO_VID_ENC_MPEG_4,
    MINFO_INFO_VID_ENC_DIVX_311,
    MINFO_INFO_VID_ENC_DIVX_4,
    MINFO_INFO_VID_ENC_DIVX_5,
    MINFO_INFO_VID_ENC_XVID,
    MINFO_INFO_VID_ENC_WMV1,
    MINFO_INFO_VID_ENC_WMV2,
    MINFO_INFO_VID_ENC_WMV3,
    MINFO_INFO_VID_ENC_WVC1,
    MINFO_INFO_VID_ENC_H264,
    MINFO_INFO_VID_ENC_H263,
    MINFO_INFO_VID_ENC_MJPG,
    MINFO_INFO_VID_ENC_RV8,
    MINFO_INFO_VID_ENC_RV9,
    MINFO_INFO_VID_ENC_SORENSON,
    MINFO_INFO_VID_ENC_AVS,
    MINFO_INFO_VID_ENC_NV12,
    MINFO_INFO_VID_ENC_VP6,
    MINFO_INFO_VID_ENC_VP8
}MINFO_INFO_VIDEO_ENC_T;

/* This structure enumerates all encoding types of subtitle stream.*/
/*------------------------------------------------------------------*/
/*! @enum MINFO_INFO_SUBTITLE_TYPE_T
 *  @brief Subtitle type.
 *  @code
 *  typedef enum
 *  {
 *      MINFO_INFO_SUBTITLE_TYPE_UNKNOWN = 0,
 *      MINFO_INFO_AVI_SUBTITLE_TYPE_TEXT,
 *      MINFO_INFO_AVI_SUBTITLE_TYPE_BITMAP,
 *      MINFO_INFO_AVI_SUBTITLE_TYPE_BITMAP_DXSA,    
 *      MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_UTF8,
 *      MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_SSA,
 *      MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_USF,
 *      MINFO_INFO_MKV_SUBTITLE_TYPE_VOBSUB,
 *      MINFO_INFO_MKV_SUBTITLE_TYPE_IMG_BMP,
 *      MINFO_INFO_MKV_SUBTITLE_TYPE_KATE
 *  }MINFO_INFO_SUBTITLE_TYPE_T;
 *  @endcode
 *  @li@c  MINFO_INFO_SUBTITLE_TYPE_UNKNOWN                  -
 *  @li@c  MINFO_INFO_AVI_SUBTITLE_TYPE_TEXT                 -
 *  @li@c  MINFO_INFO_AVI_SUBTITLE_TYPE_BITMAP               -
 *  @li@c  MINFO_INFO_AVI_SUBTITLE_TYPE_BITMAP_DXSA          -
 *  @li@c  MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_UTF8            -
 *  @li@c  MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_SSA             -
 *  @li@c  MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_USF             -
 *  @li@c  MINFO_INFO_MKV_SUBTITLE_TYPE_VOBSUB               -
 *  @li@c  MINFO_INFO_MKV_SUBTITLE_TYPE_IMG_BMP              -
 *  @li@c  MINFO_INFO_MKV_SUBTITLE_TYPE_KATE                 -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MINFO_INFO_SUBTITLE_TYPE_UNKNOWN = 0,
    MINFO_INFO_AVI_SUBTITLE_TYPE_TEXT,
    MINFO_INFO_AVI_SUBTITLE_TYPE_BITMAP,
    MINFO_INFO_AVI_SUBTITLE_TYPE_BITMAP_DXSA,    /* Divx XSUB+ */
    MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_UTF8,
    MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_SSA,
    MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_USF,
    MINFO_INFO_MKV_SUBTITLE_TYPE_VOBSUB,
    MINFO_INFO_MKV_SUBTITLE_TYPE_IMG_BMP,
    MINFO_INFO_MKV_SUBTITLE_TYPE_KATE,
    MINFO_INFO_MP4_SUBTITLE_TYPE_BLUERAY, /*add for mp4 subtitle*/
    MINFO_INFO_MP4_SUBTITLE_TYPE_VOBSBTL

}MINFO_INFO_SUBTITLE_TYPE_T;

/* This structure enumerates all  format types of video TS stream.*/
/*------------------------------------------------------------------*/
/*! @enum MINFO_INFO_VID_FMT_T
 *  @brief Video format type.
 *  @code
 *  typedef enum
 *  {
 *      MINFO_INFO_VID_FMT_UNKNOWN = 0,
 *      MINFO_INFO_VID_FMT_HD_4_3,
 *      MINFO_INFO_VID_FMT_HD_16_9,
 *      MINFO_INFO_VID_FMT_SD_4_3,
 *      MINFO_INFO_VID_FMT_SD_16_9,
 *      MINFO_INFO_VID_FMT_VD_16_9
 *  }MINFO_INFO_VID_FMT_T;
 *  @endcode
 *  @li@c  MINFO_INFO_VID_FMT_UNKNOWN                        -
 *  @li@c  MINFO_INFO_VID_FMT_HD_4_3                         -
 *  @li@c  MINFO_INFO_VID_FMT_HD_16_9                        -
 *  @li@c  MINFO_INFO_VID_FMT_SD_4_3                         -
 *  @li@c  MINFO_INFO_VID_FMT_SD_16_9                        -
 *  @li@c  MINFO_INFO_VID_FMT_VD_16_9                        -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MINFO_INFO_VID_FMT_UNKNOWN = 0,
    MINFO_INFO_VID_FMT_HD_4_3,
    MINFO_INFO_VID_FMT_HD_16_9,
    MINFO_INFO_VID_FMT_SD_4_3,
    MINFO_INFO_VID_FMT_SD_16_9,
    MINFO_INFO_VID_FMT_VD_16_9
}MINFO_INFO_VID_FMT_T;

/********* Structure for MPEG audio stream *********/
/*------------------------------------------------------------------*/
/*! @struct MINFO_MP3_AUDIO_STM_ATRBT_T
 *  @brief Audio stream attribute.
 *  @code
 *  typedef struct _MINFO_MP3_AUDIO_STM_ATRBT_T
 *  {
 *      MINFO_INFO_AUDIO_ENC_T      e_enc;
 *      MINFO_INFO_AUD_LAYER_T      e_layer;
 *      UINT32                      ui4_bitrate;
 *      UINT32                      ui4_sample_rate;
 *      UINT8                       ui1_version;
 *      UINT32                      ui4_frame_length;
 *  } MINFO_MP3_AUDIO_STM_ATRBT_T;
 *  @endcode
 *  @li@c  e_enc                                             - Encode type.
 *  @li@c  e_layer                                           - Layer.
 *  @li@c  ui4_bitrate                                       - Bitrate.
 *  @li@c  ui4_sample_rate                                   - Sample rate.
 *  @li@c  ui1_version                                       - Version.
 *  @li@c  ui4_frame_length                                  - Frame length.
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_MP3_AUDIO_STM_ATRBT_T
{
    MINFO_INFO_AUDIO_ENC_T      e_enc;
    MINFO_INFO_AUD_LAYER_T      e_layer;
    UINT32                      ui4_bitrate;
    UINT32                      ui4_sample_rate;
    UINT8                       ui1_version;
    UINT32                      ui4_frame_length;
    UINT8                       ui1_channels;
    CHAR                        s_lang[ISO_639_LANG_LEN];
} MINFO_MP3_AUDIO_STM_ATRBT_T;

/*add for flv*/
/* This structure define attributes of audio stream in FLV container*/
typedef struct _MINFO_FLV_AUDIO_STM_ATRBT_T
{
    MINFO_INFO_AUDIO_ENC_T      e_enc;
    MINFO_INFO_AUD_LAYER_T      e_layer;
    INT16                       i2_channels;
    INT32                       i4_samples_per_sec;
    INT16                       i2_bits_per_sample;
    UINT32                      ui4_avg_bytes_per_sec;
    UINT64                      ui8_duration;/*clock 90000HZ*/
    UINT32                      ui4_type;
    BOOL                        b_default;
    UINT32                      ui4_codec_info_size;
    UINT8*                      pui1_codec_info;
    UINT32                      ui4_start_pts;
    CHAR                        s_lang[ISO_639_LANG_LEN];
}MINFO_FLV_AUDIO_STM_ATRBT_T;


/* This structure define attributes of video stream in FLV container*/
typedef struct _MINFO_FLV_VIDEO_STM_ATRBT_T
{
    MINFO_INFO_VIDEO_ENC_T      e_enc;   
    UINT32                      ui4_scale;
    UINT32                      ui4_rate;
    INT32                       i4_width;
    INT32                       i4_height;
    UINT64                      ui8_duration;/*clock 90000HZ*/
    UINT32                      ui4_codec_info_size;
    UINT8*                      pui1_codec_info;
    BOOL                        b_haskeytable;
    UINT32                      ui4_start_pts;
}MINFO_FLV_VIDEO_STM_ATRBT_T;

/* This structure define attributes of video stream in CMPB container*/
typedef struct _MINFO_CMPB_VIDEO_STM_ATRBT_T
{
    MINFO_INFO_VIDEO_ENC_T      e_enc;   
    UINT8                       ui1_sample_id;
    INT32                       i4_width;
    INT32                       i4_height;
    UINT32                      ui4_scale;
    UINT32                      ui4_rate;
    /*IMTK_PB_CTRL_ASPECT_RATIO_T     eSrcAsp;*/    
    UINT32                      ui4_seq_hdr;
    UINT16                      ui2_seq_hdr_size;    
    UINT8*                      pu1_buff;
    UINT32                      ui4_buff_size;
}MINFO_CMPB_VIDEO_STM_ATRBT_T;

typedef enum
{
    MM_AUD_PCM_TYPE_NORMAL        =   1,
    MM_AUD_PCM_TYPE_MS_ADPCM,
    MM_AUD_PCM_TYPE_IMA_ADPCM
} MM_AUD_PCM_TYPE_T;

typedef struct _MINFO_CMPB_AUDIO_STM_ATRBT_T
{
    MINFO_INFO_AUDIO_ENC_T      e_enc;                  /* Audio Codec */
    UINT32                      ui4_stream_id;          /* Stream ID */
    
    UINT16                      ui2_hdr_src;            /* W_WMA_HEADER_SOURCE */
    UINT16                      ui2_channels;           /* W_WMA_NUM_CH */
    UINT32                      ui4_sample_per_sec;     /* D_WMA_SAMPLE_PER_SEC */
    UINT32                      ui4_bits_per_sec;       /* D_WMA_BYTE_PER_SEC */
    UINT32                      ui4_block_size;         /* D_WMA_BLOCK_SIZE */
    UINT16                      ui2_encoder_opt;        /* W_WMA_ENCODER_OPTION */
        
    MM_AUD_PCM_TYPE_T           e_pcm_type;             /* PCM type */
    UINT32                      ui4_sample_rate;        /* Sampling Frequence */
    UINT16                      ui2_block_align;        /* BLock Alignment */
    UINT16                      ui2_bits_per_sample;    /* Bits per Sample */
    BOOL                        fg_big_endian;          /* Big Endian PCM */
    
    UINT8                       ui1_mpg_layer;          /* Layer of mpeg audio*/
    CHAR                        s_lang[ISO_639_LANG_LEN];

}MINFO_CMPB_AUDIO_STM_ATRBT_T;

typedef enum
{
    CMPB_DVB_SBTL_UNKNOWN = 0,
    CMPB_DVB_SBTL_NO_ASP_RATIO,
    CMPB_DVB_SBTL_4_3,
    CMPB_DVB_SBTL_16_9,
    CMPB_DVB_SBTL_221_1,
    CMPB_DVB_SBTL_HD,
    CMPB_DVB_SBTL_NO_ASP_RATIO_HOH,
    CMPB_DVB_SBTL_4_3_HOH,
    CMPB_DVB_SBTL_16_9_HOH,
    CMPB_DVB_SBTL_221_1_HOH,
    CMPB_DVB_SBTL_HD_HOH
} CMPB_DVB_SBTL_TYPE_T;

typedef struct _MINFO_CMPB_SUBTITLE_STM_ATRBT_T
{
    MPEG_2_PID_T               ui2_pid;
    UINT16                     ui2_comp_pg_id;
    UINT16                     ui2_anci_pg_id;
    CHAR                       s_lang [ISO_639_LANG_LEN];
    CMPB_DVB_SBTL_TYPE_T       e_sbtl_type;
    UINT16                     ui2_pmt_index;
    HANDLE_T                   h_gl_plane;
}MINFO_CMPB_SUBTL_STM_ATRBT_T;

/*RM info*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_RM_VIDEO_STM_ATRBT_T
 *  @brief Rm video stream attribute.
 *  @code
 *  typedef struct _MINFO_RM_VIDEO_STM_ATRBT_T
 *  {
 *      MINFO_INFO_VIDEO_ENC_T      e_enc;
 *      UINT16  ui2_strm_num;
 *      UINT32  ui4_preroll;
 *      UINT32  ui4_duration;
 *      UINT32  ui4_max_bit_rate;
 *      UINT32  ui4_avg_bit_rate;
 *      UINT32  ui4_max_packet_size;
 *      UINT32  ui4_avg_packet_size;
 *      UINT32  ui4_start_time;
 *      UINT32  ui4_codec_id;
 *      UINT16  ui2_frm_width;
 *      UINT16  ui2_frm_height;
 *      UINT16  ui2_bit_count;
 *      UINT16  ui2_pad_width;
 *      UINT16  ui2_pad_height;
 *      UINT32  ui4_frm_rate;
 *      UINT32  ui4_spo_extra_flags;
 *      UINT32  ui4_version;
 *      UINT32  ui4_specific;  
 *      UINT8   ui1_ecc_mask;
 *  } MINFO_RM_VIDEO_STM_ATRBT_T;
 *  @endcode
 *  @li@c  e_enc                                             -
 *  @li@c  ui2_strm_num                                      -
 *  @li@c  ui4_preroll                                       -
 *  @li@c  ui4_duration                                      -
 *  @li@c  ui4_max_bit_rate                                  -
 *  @li@c  ui4_avg_bit_rate                                  -
 *  @li@c  ui4_max_packet_size                               -
 *  @li@c  ui4_avg_packet_size                               -
 *  @li@c  ui4_start_time                                    -
 *  @li@c  ui4_codec_id                                      -
 *  @li@c  ui2_frm_width                                     -
 *  @li@c  ui2_frm_height                                    -
 *  @li@c  ui2_bit_count                                     -
 *  @li@c  ui2_pad_width                                     -
 *  @li@c  ui2_pad_height                                    -
 *  @li@c  ui4_frm_rate                                      -
 *  @li@c  ui4_spo_extra_flags                               -
 *  @li@c  ui4_version                                       -
 *  @li@c  ui4_specific                                      -
 *  @li@c  ui1_ecc_mask                                      -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_RM_VIDEO_STM_ATRBT_T
{
    MINFO_INFO_VIDEO_ENC_T      e_enc;
    UINT16  ui2_strm_num;
    UINT32  ui4_preroll;
    UINT32  ui4_duration;
    UINT32  ui4_max_bit_rate;
    UINT32  ui4_avg_bit_rate;
    UINT32  ui4_max_packet_size;
    UINT32  ui4_avg_packet_size;
    UINT32  ui4_start_time;
    UINT32  ui4_codec_id;
    UINT16  ui2_frm_width;
    UINT16  ui2_frm_height;
    UINT16  ui2_bit_count;
    UINT16  ui2_pad_width;
    UINT16  ui2_pad_height;
    UINT32  ui4_frm_rate;
    UINT32  ui4_spo_extra_flags;
    UINT32  ui4_version;
    UINT32  ui4_specific;  
    UINT8   ui1_ecc_mask;
} MINFO_RM_VIDEO_STM_ATRBT_T;

/*------------------------------------------------------------------*/
/*! @struct MINFO_RM_AUDIO_STM_ATRBT_T
 *  @brief RM audio stream attribute.
 *  @code
 *  typedef struct _MINFO_RM_AUDIO_STM_ATRBT_T
 *  {
 *      MINFO_INFO_AUDIO_ENC_T      e_enc;
 *      UINT16  ui2_strm_num;
 *      UINT32  ui4_preroll;
 *      UINT32  ui4_duration;
 *      UINT32  ui4_max_bit_rate;
 *      UINT32  ui4_avg_bit_rate;
 *      UINT32  ui4_max_packet_size;
 *      UINT32  ui4_avg_packet_size;
 *      UINT32  ui4_start_time;
 *      UINT16  ui2_version;
 *      UINT16  ui2_revision;
 *      UINT16  ui2_hdr_bytes;
 *      UINT16  ui2_flavor_idx;
 *      UINT32  ui4_granularity;
 *      UINT32  ui4_total_bytes;
 *      UINT32  ui4_bytes_per_min;
 *      UINT32  ui4_bytes_per_min2;
 *      UINT16  ui2_intlv_factor;
 *      UINT16  ui2_intlv_blk_sz;
 *      UINT16  ui2_codec_frm_sz;
 *      UINT32  ui4_user_data;
 *      UINT32  ui4_sample_rate;
 *      UINT32  ui4_act_sample_rate;
 *      UINT16  ui2_sample_sz;
 *      UINT16  ui2_num_channel;
 *      UINT32  ui4_intlvr_id;
 *      UINT32  ui4_codec_id;
 *      UINT8   ui1_intlv_flag;
 *      UINT8   ui1_can_copy_flag;
 *      UINT8   ui1_strm_type;
 *      UINT8   ui1_has_intlv_ptrn;
 *      void    *pv_intlv_ptrn;
 *      UINT32  ui4_opaque_data_sz;
 *      void    *pv_codec_opaque_data;
 *  } MINFO_RM_AUDIO_STM_ATRBT_T;
 *  @endcode
 *  @li@c  e_enc                                             -
 *  @li@c  ui2_strm_num                                      -
 *  @li@c  ui4_preroll                                       -
 *  @li@c  ui4_duration                                      -
 *  @li@c  ui4_max_bit_rate                                  -
 *  @li@c  ui4_avg_bit_rate                                  -
 *  @li@c  ui4_max_packet_size                               -
 *  @li@c  ui4_avg_packet_size                               -
 *  @li@c  ui4_start_time                                    -
 *  @li@c  ui2_version                                       -
 *  @li@c  ui2_revision                                      -
 *  @li@c  ui2_hdr_bytes                                     -
 *  @li@c  ui2_flavor_idx                                    -
 *  @li@c  ui4_granularity                                   -
 *  @li@c  ui4_total_bytes                                   -
 *  @li@c  ui4_bytes_per_min                                 -
 *  @li@c  ui4_bytes_per_min2                                -
 *  @li@c  ui2_intlv_factor                                  -
 *  @li@c  ui2_intlv_blk_sz                                  -
 *  @li@c  ui2_codec_frm_sz                                  -
 *  @li@c  ui4_user_data                                     -
 *  @li@c  ui4_sample_rate                                   -
 *  @li@c  ui4_act_sample_rate                               -
 *  @li@c  ui2_sample_sz                                     -
 *  @li@c  ui2_num_channel                                   -
 *  @li@c  ui4_intlvr_id                                     -
 *  @li@c  ui4_codec_id                                      -
 *  @li@c  ui1_intlv_flag                                    -
 *  @li@c  ui1_can_copy_flag                                 -
 *  @li@c  ui1_strm_type                                     -
 *  @li@c  ui1_has_intlv_ptrn                                -
 *  @li@c  *pv_intlv_ptrn                                    -
 *  @li@c  ui4_opaque_data_sz                                -
 *  @li@c  *pv_codec_opaque_data                             -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_RM_AUDIO_STM_ATRBT_T
{
    MINFO_INFO_AUDIO_ENC_T      e_enc;
    UINT16  ui2_strm_num;
    UINT32  ui4_preroll;
    UINT32  ui4_duration;
    UINT32  ui4_max_bit_rate;
    UINT32  ui4_avg_bit_rate;
    UINT32  ui4_max_packet_size;
    UINT32  ui4_avg_packet_size;
    UINT32  ui4_start_time;
    UINT16  ui2_version;
    UINT16  ui2_revision;
    UINT16  ui2_hdr_bytes;
    UINT16  ui2_flavor_idx;
    UINT32  ui4_granularity;
    UINT32  ui4_total_bytes;
    UINT32  ui4_bytes_per_min;
    UINT32  ui4_bytes_per_min2;
    UINT16  ui2_intlv_factor;
    UINT16  ui2_intlv_blk_sz;
    UINT16  ui2_codec_frm_sz;
    UINT32  ui4_user_data;
    UINT32  ui4_sample_rate;
    UINT32  ui4_act_sample_rate;
    UINT16  ui2_sample_sz;
    UINT16  ui2_num_channel;
    UINT32  ui4_intlvr_id;
    UINT32  ui4_codec_id;
    UINT8   ui1_intlv_flag;
    UINT8   ui1_can_copy_flag;
    UINT8   ui1_strm_type;
    UINT8   ui1_has_intlv_ptrn;
    void    *pv_intlv_ptrn;
    UINT32  ui4_opaque_data_sz;
    void    *pv_codec_opaque_data;
    CHAR    s_lang[ISO_639_LANG_LEN];
} MINFO_RM_AUDIO_STM_ATRBT_T;

/*AIF info*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_AIF_AUDIO_STM_ATRBT_T
 *  @brief Aif audio stream infor.
 *  @code
 *  typedef struct _MINFO_AIF_AUDIO_STM_ATRBT_T
 *  {
 *      MINFO_INFO_AUDIO_ENC_T      e_enc;
 *      UINT16                      ui2_num_channels;
 *      UINT32                      ui4_num_sample_frames;
 *      UINT16                      ui2_sample_size;
 *      UINT32                      ui4_sample_rate;
 *      UINT32                      ui4_sample_rate_scale;
 *  } MINFO_AIF_AUDIO_STM_ATRBT_T;
 *  @endcode
 *  @li@c  e_enc                                             -
 *  @li@c  ui2_num_channels                                  -
 *  @li@c  ui4_num_sample_frames                             -
 *  @li@c  ui2_sample_size                                   -
 *  @li@c  ui4_sample_rate                                   -
 *  @li@c  ui4_sample_rate_scale                             -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_AIF_AUDIO_STM_ATRBT_T
{
    MINFO_INFO_AUDIO_ENC_T      e_enc;
    UINT16                      ui2_num_channels;
    UINT32                      ui4_num_sample_frames;
    UINT16                      ui2_sample_size;
    UINT32                      ui4_sample_rate;
    UINT32                      ui4_sample_rate_scale;
    CHAR                        s_lang[ISO_639_LANG_LEN];
} MINFO_AIF_AUDIO_STM_ATRBT_T;

/* This structure define attributes of audio stream in AVI container*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_AVI_AUDIO_STM_ATRBT_T
 *  @brief Avi audio stream infor.
 *  @code
 *  typedef struct _MINFO_AVI_AUDIO_STM_ATRBT_T
 *  {
 *      MINFO_INFO_AUDIO_ENC_T      e_enc;
 *      MINFO_INFO_AUD_LAYER_T      e_layer;
 *      UINT32                      ui4_scale;
 *      UINT32                      ui4_rate;
 *      UINT32                      ui4_sample_size;
 *      INT16                       i2_channels;
 *      INT32                       i4_samples_per_sec;
 *      INT16                       i2_block_align;
 *      INT16                       i2_bits_per_sample;
 *      UINT32                      ui4_avg_bytes_sec;
 *      UINT32                      ui4_length;
 *      UINT64                      ui8_duration;
 *      UINT16                      ui2_encode_opts;
 *      CHAR                        s_lang[ISO_639_LANG_LEN];
 *      BOOL                        b_default;
 *      UINT32                      ui4_codec_info_size;
 *      UINT8*                      pui1_codec_info;
 *      UINT32                      ui4_indx_offset;
 *      UINT32                      ui4_indx_size;
 *      UINT32                      ui4_cbr_sub_vbr;
 *  }MINFO_AVI_AUDIO_STM_ATRBT_T;
 *  @endcode
 *  @li@c  e_enc                                             -
 *  @li@c  e_layer                                           -
 *  @li@c  ui4_scale                                         -
 *  @li@c  ui4_rate                                          -
 *  @li@c  ui4_sample_size                                   -
 *  @li@c  i2_channels                                       -
 *  @li@c  i4_samples_per_sec                                -
 *  @li@c  i2_block_align                                    -
 *  @li@c  i2_bits_per_sample                                -
 *  @li@c  ui4_avg_bytes_sec                                 -
 *  @li@c  ui4_length                                        -
 *  @li@c  ui8_duration                                      -
 *  @li@c  ui2_encode_opts                                   -
 *  @li@c  s_lang[ISO_639_LANG_LEN]                          -
 *  @li@c  b_default                                         -
 *  @li@c  ui4_codec_info_size                               -
 *  @li@c  pui1_codec_info                                   -
 *  @li@c  ui4_indx_offset                                   -
 *  @li@c  ui4_indx_size                                     -
 *  @li@c  ui4_cbr_sub_vbr                                   -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_AVI_AUDIO_STM_ATRBT_T
{
    MINFO_INFO_AUDIO_ENC_T      e_enc;
    MINFO_INFO_AUD_LAYER_T      e_layer;
    UINT32                      ui4_scale;
    UINT32                      ui4_rate;
    UINT32                      ui4_sample_size;
    INT16                       i2_channels;
    INT32                       i4_samples_per_sec;
    INT16                       i2_block_align;
    INT16                       i2_bits_per_sample;
    UINT32                      ui4_avg_bytes_sec;
    UINT32                      ui4_aac_profile;            /* the profile for aac */
    UINT32                      ui4_length;                 /*defined in AVI spec*/
    UINT64                      ui8_duration;               /*clock 90000HZ*/
    UINT16                      ui2_encode_opts;            /*for WMA*/
    CHAR                        s_lang[ISO_639_LANG_LEN];
    BOOL                        b_default;
    UINT32                      ui4_codec_info_size;
    UINT8*                      pui1_codec_info;
    UINT32                      ui4_indx_offset;
    UINT32                      ui4_indx_size;
    UINT32                      ui4_cbr_sub_vbr;
}MINFO_AVI_AUDIO_STM_ATRBT_T;

/* This structure define attributes of video stream in AVI container*/
/*------------------------------------------------------------------*/
/*! @enum MINFO_AVI_VIDEO_CODEC_INFO_TYPE_T
 *  @brief Avi video codec type.
 *  @code
 *  typedef enum
 *  {
 *      MINFO_AVI_VIDEO_CODEC_INFO_TYPE_UNKNOWN  = 0,
 *      MINFO_AVI_VIDEO_CODEC_INFO_TYPE_XXDB
 *  }MINFO_AVI_VIDEO_CODEC_INFO_TYPE_T;
 *  @endcode
 *  @li@c  MINFO_AVI_VIDEO_CODEC_INFO_TYPE_UNKNOWN           -
 *  @li@c  MINFO_AVI_VIDEO_CODEC_INFO_TYPE_XXDB              -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MINFO_AVI_VIDEO_CODEC_INFO_TYPE_UNKNOWN  = 0,
    MINFO_AVI_VIDEO_CODEC_INFO_TYPE_XXDB
}MINFO_AVI_VIDEO_CODEC_INFO_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct MINFO_AVI_VIDEO_CODEC_XXDB_T
 *  @brief Video codec xxdb.
 *  @code
 *  typedef struct _MINFO_AVI_VIDEO_CODEC_XXDB_T
 *  {
 *      UINT32                      ui4_size;
 *      UINT32                      ui4_offset;
 *  }MINFO_AVI_VIDEO_CODEC_XXDB_T;
 *  @endcode
 *  @li@c  ui4_size                                          -
 *  @li@c  ui4_offset                                        -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_AVI_VIDEO_CODEC_XXDB_T
{
    UINT32                      ui4_size;
    UINT32                      ui4_offset;
}MINFO_AVI_VIDEO_CODEC_XXDB_T;

/*------------------------------------------------------------------*/
/*! @struct MINFO_AVI_VIDEO_CODEC_INFO_T
 *  @brief Avi video codec infor.
 *  @code
 *  typedef struct _MINFO_AVI_VIDEO_CODEC_INFO_T
 *  {
 *      MINFO_AVI_VIDEO_CODEC_INFO_TYPE_T   e_type;
 *      union
 *      {
 *         MINFO_AVI_VIDEO_CODEC_XXDB_T     t_avi_vid_codec_xxdb;
 *      }u;
 *  }MINFO_AVI_VIDEO_CODEC_INFO_T;
 *  @endcode
 *  @li@c  e_type                                            -
 *  @li@c  t_avi_vid_codec_xxdb                              -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_AVI_VIDEO_CODEC_INFO_T
{
    MINFO_AVI_VIDEO_CODEC_INFO_TYPE_T   e_type;
    union
    {
       MINFO_AVI_VIDEO_CODEC_XXDB_T     t_avi_vid_codec_xxdb;
    }u;

}MINFO_AVI_VIDEO_CODEC_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct MINFO_AVI_VIDEO_STM_ATRBT_T
 *  @brief Avi video stream infor.
 *  @code
 *  typedef struct _MINFO_AVI_VIDEO_STM_ATRBT_T
 *  {
 *      MINFO_INFO_VIDEO_ENC_T      e_enc;
 *      UINT32                      ui4_scale;
 *      UINT32                      ui4_rate;
 *      UINT32                      ui4_sample_size;
 *      INT32                       i4_width;
 *      INT32                       i4_height;
 *      UINT32                      ui4_length;
 *      UINT64                      ui8_duration;
 *      BOOL                        b_default;
 *      UINT32                      ui4_codec_info_size;
 *      UINT8*                      pui1_codec_info;
 *      UINT32                      ui4_indx_offset;
 *      UINT32                      ui4_indx_size;
 *  }MINFO_AVI_VIDEO_STM_ATRBT_T;
 *  @endcode
 *  @li@c  e_enc                                             -
 *  @li@c  ui4_scale                                         -
 *  @li@c  ui4_rate                                          -
 *  @li@c  ui4_sample_size                                   -
 *  @li@c  i4_width                                          -
 *  @li@c  i4_height                                         -
 *  @li@c  ui4_length                                        -
 *  @li@c  ui8_duration                                      -
 *  @li@c  b_default                                         -
 *  @li@c  ui4_codec_info_size                               -
 *  @li@c  pui1_codec_info                                   -
 *  @li@c  ui4_indx_offset                                   -
 *  @li@c  ui4_indx_size                                     -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_AVI_VIDEO_STM_ATRBT_T
{
    MINFO_INFO_VIDEO_ENC_T      e_enc;
    UINT32                      ui4_scale;
    UINT32                      ui4_rate;
    UINT32                      ui4_sample_size;
    INT32                       i4_width;
    INT32                       i4_height;
    UINT32                      ui4_length;/*defined in AVI spec*/
    UINT64                      ui8_duration;/*clock 90000HZ*/
    BOOL                        b_default;
    UINT32                      ui4_codec_info_size;
    UINT8*                      pui1_codec_info;
    UINT32                      ui4_indx_offset;
    UINT32                      ui4_indx_size;
    UINT32                      ui4_ext_data;
    CHAR                        ac_3d_identifier[21];
    CHAR                        ac_3d_version[5];
}MINFO_AVI_VIDEO_STM_ATRBT_T;

/* This structure define attributes of subtitle stream in AVI container*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_AVI_DIVX_SUBTITLE_STM_ATRBT_T
 *  @brief Avi divx subtitle stream infor.
 *  @code
 *  typedef struct _MINFO_AVI_DIVX_SUBTITLE_STM_ATRBT_T
 *  {
 *      MINFO_INFO_SUBTITLE_TYPE_T   ui4_subtitle_type;
 *      UINT32                       ui4_scale;
 *      UINT32                       ui4_rate;
 *      UINT32                       ui4_sample_size;
 *      INT32                        i4_width;
 *      INT32                        i4_height;
 *      CHAR                         s_lang[ISO_639_LANG_LEN];
 *      BOOL                         b_default;
 *      UINT32                       ui4_indx_offset;
 *      UINT32                       ui4_indx_size;
 *  }MINFO_AVI_DIVX_SUBTITLE_STM_ATRBT_T;
 *  @endcode
 *  @li@c  ui4_subtitle_type                                 -
 *  @li@c  ui4_scale                                         -
 *  @li@c  ui4_rate                                          -
 *  @li@c  ui4_sample_size                                   -
 *  @li@c  i4_width                                          -
 *  @li@c  i4_height                                         -
 *  @li@c  s_lang[ISO_639_LANG_LEN]                          -
 *  @li@c  b_default                                         -
 *  @li@c  ui4_indx_offset                                   -
 *  @li@c  ui4_indx_size                                     -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_AVI_DIVX_SUBTITLE_STM_ATRBT_T
{
    MINFO_INFO_SUBTITLE_TYPE_T   ui4_subtitle_type;
    UINT32                       ui4_scale;
    UINT32                       ui4_rate;
    UINT32                       ui4_sample_size;
    INT32                        i4_width;
    INT32                        i4_height;
    CHAR                         s_lang[ISO_639_LANG_LEN];
    BOOL                         b_default;
    UINT32                       ui4_indx_offset;
    UINT32                       ui4_indx_size;
}MINFO_AVI_DIVX_SUBTITLE_STM_ATRBT_T;

/* This structure define attributes of audio stream in ASF container*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_ASF_AUDIO_STM_ATRBT_T
 *  @brief Asf audio stream infor.
 *  @code
 *  typedef struct _MINFO_ASF_AUDIO_STM_ATRBT_T
 *  {
 *      MINFO_INFO_AUDIO_ENC_T      e_enc;
 *      MINFO_INFO_AUD_LAYER_T      e_layer;
 *      INT16                       i2_channels;
 *      INT32                       i4_samples_per_sec;
 *      INT16                       i2_block_align;
 *      INT16                       i2_bits_per_sample;
 *      UINT32                      ui4_avg_bytes_per_sec;
 *      CHAR                        s_lang[ISO_639_LANG_LEN];
 *      BOOL                        b_default;
 *      UINT32                      ui4_codec_info_size;
 *      UINT8*                      pui1_codec_info;
 *      UINT32                      ui4_packet_sz;
 *  }MINFO_ASF_AUDIO_STM_ATRBT_T;
 *  @endcode
 *  @li@c  e_enc                                             -
 *  @li@c  e_layer                                           -
 *  @li@c  i2_channels                                       -
 *  @li@c  i4_samples_per_sec                                -
 *  @li@c  i2_block_align                                    -
 *  @li@c  i2_bits_per_sample                                -
 *  @li@c  ui4_avg_bytes_per_sec                             -
 *  @li@c  s_lang[ISO_639_LANG_LEN]                          -
 *  @li@c  b_default                                         -
 *  @li@c  ui4_codec_info_size                               -
 *  @li@c  pui1_codec_info                                   -
 *  @li@c  ui4_packet_sz                                     -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_ASF_AUDIO_STM_ATRBT_T
{
    MINFO_INFO_AUDIO_ENC_T      e_enc;
    MINFO_INFO_AUD_LAYER_T      e_layer;
    INT16                       i2_channels;
    INT32                       i4_samples_per_sec;
    INT16                       i2_block_align;
    INT16                       i2_bits_per_sample;
    UINT32                      ui4_avg_bytes_per_sec;
    CHAR                        s_lang[ISO_639_LANG_LEN];
    BOOL                        b_default;
    UINT32                      ui4_codec_info_size;
    UINT8*                      pui1_codec_info;
    UINT32                      ui4_packet_sz;
}MINFO_ASF_AUDIO_STM_ATRBT_T;


/* This structure define attributes of video stream in ASF container*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_ASF_VIDEO_STM_ATRBT_T
 *  @brief Asf video stream infor.
 *  @code
 *  typedef struct _MINFO_ASF_VIDEO_STM_ATRBT_T
 *  {
 *      MINFO_INFO_VIDEO_ENC_T      e_enc;   
 *      UINT32                      ui4_scale;
 *      UINT32                      ui4_rate;
 *      INT32                       i4_width;
 *      INT32                       i4_height;
 *      BOOL                        b_default;
 *      UINT32                      ui4_codec_info_size;
 *      UINT8*                      pui1_codec_info;
 *      UINT32                      ui4_packet_sz;    
 *      UINT8                       ui1_sample_id;
 *  }MINFO_ASF_VIDEO_STM_ATRBT_T;
 *  @endcode
 *  @li@c  e_enc                                             -
 *  @li@c  ui4_scale                                         -
 *  @li@c  ui4_rate                                          -
 *  @li@c  i4_width                                          -
 *  @li@c  i4_height                                         -
 *  @li@c  b_default                                         -
 *  @li@c  ui4_codec_info_size                               -
 *  @li@c  pui1_codec_info                                   -
 *  @li@c  ui4_packet_sz                                     -
 *  @li@c  ui1_sample_id                                     -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_ASF_VIDEO_STM_ATRBT_T
{
    MINFO_INFO_VIDEO_ENC_T      e_enc;   
    UINT32                      ui4_scale;
    UINT32                      ui4_rate;
    INT32                       i4_width;
    INT32                       i4_height;
    BOOL                        b_default;
    UINT32                      ui4_codec_info_size;
    UINT8*                      pui1_codec_info;
    UINT32                      ui4_packet_sz;    
    UINT8                       ui1_sample_id;
}MINFO_ASF_VIDEO_STM_ATRBT_T;


/* This structure define attributes of audio stream in MP4 container*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_MP4_AUDIO_STM_ATRBT_T
 *  @brief Mp4 audio stream infor.
 *  @code
 *  typedef struct _MINFO_MP4_AUDIO_STM_ATRBT_T
 *  {
 *      MINFO_INFO_AUDIO_ENC_T      e_enc;
 *      MINFO_INFO_AUD_LAYER_T      e_layer;
 *      INT16                       i2_channels;
 *      INT32                       i4_sample_rate;
 *      INT32                       i4_sample_size;
 *      UINT32                      ui4_timescale;
 *      BOOL                        b_default;
 *      UINT32                      ui4_codec_info_size;
 *      UINT8*                      pui1_codec_info;
 *  }MINFO_MP4_AUDIO_STM_ATRBT_T;
 *  @endcode
 *  @li@c  e_enc                                             -
 *  @li@c  e_layer                                           -
 *  @li@c  i2_channels                                       -
 *  @li@c  i4_sample_rate                                    -
 *  @li@c  i4_sample_size                                    -
 *  @li@c  ui4_timescale                                     -
 *  @li@c  b_default                                         -
 *  @li@c  ui4_codec_info_size                               -
 *  @li@c  pui1_codec_info                                   -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_MP4_AUDIO_STM_ATRBT_T
{
    MINFO_INFO_AUDIO_ENC_T      e_enc;
    MINFO_INFO_AUD_LAYER_T      e_layer;
    INT16                       i2_channels;
    INT32                       i4_sample_rate;
    INT32                       i4_sample_size;
    UINT32                      ui4_aac_profile;
    UINT32                      ui4_timescale;
    BOOL                        b_default;
    UINT32                      ui4_codec_info_size;
    UINT8*                      pui1_codec_info;
    CHAR                        s_lang[ISO_639_LANG_LEN];
    BOOL                        b_frame_length;
}MINFO_MP4_AUDIO_STM_ATRBT_T;


/* This structure define attributes of video stream in MP4 container*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_MP4_VIDEO_STM_ATRBT_T
 *  @brief Mp4 video stream infor.
 *  @code
 *  typedef struct _MINFO_MP4_VIDEO_STM_ATRBT_T
 *  {
 *      MINFO_INFO_VIDEO_ENC_T      e_enc;
 *      INT32                       i4_width;
 *      INT32                       i4_height;
 *      UINT32                      ui4_timescale;
 *      UINT32                      ui4_scale;
 *      UINT32                      ui4_rate;
 *      BOOL                        b_default;
 *      UINT32                      ui4_codec_info_size;
 *      UINT8*                      pui1_codec_info;
 *  }MINFO_MP4_VIDEO_STM_ATRBT_T;
 *  @endcode
 *  @li@c  e_enc                                             -
 *  @li@c  i4_width                                          -
 *  @li@c  i4_height                                         -
 *  @li@c  ui4_timescale                                     -
 *  @li@c  ui4_scale                                         -
 *  @li@c  ui4_rate                                          -
 *  @li@c  b_default                                         -
 *  @li@c  ui4_codec_info_size                               -
 *  @li@c  pui1_codec_info                                   -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_MP4_VIDEO_STM_ATRBT_T
{
    MINFO_INFO_VIDEO_ENC_T      e_enc;
    INT32                       i4_width;
    INT32                       i4_height;
    UINT32                      ui4_timescale;
    UINT32                      ui4_scale;/*used to store frame rate == 1000*/
    UINT32                      ui4_rate;
    BOOL                        b_default;
    UINT32                      ui4_codec_info_size;
    UINT8*                      pui1_codec_info;
    UINT32                      ui4_mvcC_codec_info_size;/*For 3D mp4 mvcC*/
    UINT8*                      pui1_mvcC_codec_info;
}MINFO_MP4_VIDEO_STM_ATRBT_T;

/* This structure define attributes of subtitle  stream in MP4 container*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_MP4_SUBTITLE_STM_ATRBT_T
 *  @brief Mp4 subtitle stream infor.
 *  @code
 *  typedef struct _MINFO_MP4_SUBTITLE_STM_ATRBT_T
 *  {
 *      UINT32                      ui4_reserved;
 *  }MINFO_MP4_SUBTITLE_STM_ATRBT_T;
 *  @endcode
 *  @li@c  ui4_reserved                                      -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_MP4_SUBTITLE_STM_ATRBT_T
{
    MINFO_INFO_SUBTITLE_TYPE_T   e_subtitle_type;
    INT32                       i4_width;
    INT32                       i4_height;
    UINT32                      ui4_timescale;
    UINT32                      ui4_scale;    

}MINFO_MP4_SUBTITLE_STM_ATRBT_T;

/*float timecode scale struct, for mkv track timecode scale*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_MKV_TM_SCALE_FLOAT_T
 *  @brief Mkv timescale float value.
 *  @code
 *  typedef struct _MINFO_MKV_TM_SCALE_FLOAT_T
 *  {
 *      UINT32 ui4_float_numerator;
 *      UINT32 ui4_float_denominator;
 *  }MINFO_MKV_TM_SCALE_FLOAT_T;
 *  @endcode
 *  @li@c  ui4_float_numerator                               -
 *  @li@c  ui4_float_denominator                             -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_MKV_TM_SCALE_FLOAT_T
{
    UINT32 ui4_float_numerator;
    UINT32 ui4_float_denominator;
}MINFO_MKV_TM_SCALE_FLOAT_T;

/* This structure define attributes of subtitle stream in MKV container*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_MKV_CONTENT_ENCODING_T
 *  @brief Mkv content encoding infor.
 *  @code
 *  typedef struct _MINFO_MKV_CONTENT_ENCODING_T
 *  {
 *      UINT64       ui8_order; 
 *      UINT64       ui8_scope; 
 *      UINT64       ui8_type; 
 *      UINT64       ui8_compress_algo; 
 *      UINT64       ui8_comp_settings_length; 
 *      UINT8*       pui1_comp_settings_data; 
 *      struct _MINFO_MKV_CONTENT_ENCODING_T* pt_next;
 *  }MINFO_MKV_CONTENT_ENCODING_T;
 *  @endcode
 *  @li@c  ui8_order                                         -
 *  @li@c  ui8_scope                                         -
 *  @li@c  ui8_type                                          -
 *  @li@c  ui8_compress_algo                                 -
 *  @li@c  ui8_comp_settings_length                          -
 *  @li@c  pui1_comp_settings_data                           -
 *  @li@c  pt_next                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_MKV_CONTENT_ENCODING_T
{
    UINT64       ui8_order; 
    UINT64       ui8_scope; 
    UINT64       ui8_type; 
    UINT64       ui8_compress_algo; 
    UINT64       ui8_comp_settings_length; 
    UINT8*       pui1_comp_settings_data; 
    struct _MINFO_MKV_CONTENT_ENCODING_T* pt_next;
}MINFO_MKV_CONTENT_ENCODING_T;

/* This structure define attributes of audio stream in MKV container*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_MKV_AUDIO_STM_ATRBT_T
 *  @brief Mkv audio stream infor.
 *  @code
 *  typedef struct _MINFO_MKV_AUDIO_STM_ATRBT_T
 *  {
 *      MINFO_INFO_AUDIO_ENC_T      e_enc;
 *      MINFO_INFO_AUD_LAYER_T      e_layer;
 *      MINFO_INFO_AUD_PCM_ALIGN    e_align;
 *      UINT32                      ui4_scale;
 *      UINT32                      ui4_rate;
 *      MINFO_MKV_TM_SCALE_FLOAT_T  t_track_timecode_scale;
 *      INT16                       i2_channels;
 *      INT32                       i4_samples_per_sec;
 *      INT16                       i2_block_align;
 *      INT16                       i2_bits_per_sample;
 *      UINT32                      ui4_avg_bytes_sec;
 *      UINT16                      ui2_encode_opts;
 *      CHAR                        s_lang[ISO_639_LANG_LEN];
 *      BOOL                        b_default;
 *      UINT32                      ui4_codec_info_size;
 *      UINT8*                      pui1_codec_info;
 *  }MINFO_MKV_AUDIO_STM_ATRBT_T;
 *  @endcode
 *  @li@c  e_enc                                             -
 *  @li@c  e_layer                                           -
 *  @li@c  e_align                                           -
 *  @li@c  ui4_scale                                         -
 *  @li@c  ui4_rate                                          -
 *  @li@c  t_track_timecode_scale                            -
 *  @li@c  i2_channels                                       -
 *  @li@c  i4_samples_per_sec                                -
 *  @li@c  i2_block_align                                    -
 *  @li@c  i2_bits_per_sample                                -
 *  @li@c  ui4_avg_bytes_sec                                 -
 *  @li@c  ui2_encode_opts                                   -
 *  @li@c  s_lang[ISO_639_LANG_LEN]                          -
 *  @li@c  b_default                                         -
 *  @li@c  ui4_codec_info_size                               -
 *  @li@c  pui1_codec_info                                   -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_MKV_AUDIO_STM_ATRBT_T
{
    MINFO_INFO_AUDIO_ENC_T      e_enc;
    MINFO_INFO_AUD_LAYER_T      e_layer;
    MINFO_INFO_AUD_PCM_ALIGN    e_align;
    UINT32                      ui4_scale;
    UINT32                      ui4_rate;
    MINFO_MKV_TM_SCALE_FLOAT_T  t_track_timecode_scale;
    /*UINT32                    ui4_sample_size;*/
    INT16                       i2_channels;
    INT32                       i4_samples_per_sec;
    INT16                       i2_block_align;
    INT16                       i2_bits_per_sample;
    UINT32                      ui4_avg_bytes_sec;
    UINT32                      ui4_aac_profile;
    /*UINT32                    ui4_length;*/
    /*UINT64                    ui8_duration;*/
    UINT16                      ui2_encode_opts;
    CHAR                        s_lang[ISO_639_LANG_LEN];
    BOOL                        b_default;
#if 1/*MKV DivXPlus*/
    BOOL                        b_divx_plus;
#endif
    UINT32                      ui4_codec_info_size;
    UINT8*                      pui1_codec_info;
    
    /* add for content encodings */
    BOOL                            b_has_decode_setting;
    UINT32                          ui4_encoding_num;
    MINFO_MKV_CONTENT_ENCODING_T*   pt_content_encoding;
    /*add for codec private*/
    BOOL                            b_has_codec_private;
    UINT64                          ui8_codec_private_len;
    UINT8*                          pui1_codec_private;
}MINFO_MKV_AUDIO_STM_ATRBT_T;

/*This structure define mkv video codec type*/
/*------------------------------------------------------------------*/
/*! @enum MINFO_MKV_VIDEO_CODEC_TYPE_T
 *  @brief Mkv video codec type.
 *  @code
 *  typedef enum
 *  {
 *      MINFO_MKV_V_CODEC_TYPE_UNKNOWN = 0,
 *      MINFO_MKV_V_CODEC_TYPE_MS_VFW_FOURCC,
 *      MINFO_MKV_V_CODEC_TYPE_MPEG4_ISO_AVC,
 *      MINFO_MKV_V_CODEC_TYPE_MPEG4_ISO_SP,
 *      MINFO_MKV_V_CODEC_TYPE_MPEG4_ISO_ASP,
 *      MINFO_MKV_V_CODEC_TYPE_MPEG4_MS_V3,
 *      MINFO_MKV_V_CODEC_TYPE_MPEG1,
 *      MINFO_MKV_V_CODEC_TYPE_MPEG2
 *  }MINFO_MKV_VIDEO_CODEC_TYPE_T;
 *  @endcode
 *  @li@c  MINFO_MKV_V_CODEC_TYPE_UNKNOWN                    -
 *  @li@c  MINFO_MKV_V_CODEC_TYPE_MS_VFW_FOURCC              -
 *  @li@c  MINFO_MKV_V_CODEC_TYPE_MPEG4_ISO_AVC              -
 *  @li@c  MINFO_MKV_V_CODEC_TYPE_MPEG4_ISO_SP               -
 *  @li@c  MINFO_MKV_V_CODEC_TYPE_MPEG4_ISO_ASP              -
 *  @li@c  MINFO_MKV_V_CODEC_TYPE_MPEG4_MS_V3                -
 *  @li@c  MINFO_MKV_V_CODEC_TYPE_MPEG1                      -
 *  @li@c  MINFO_MKV_V_CODEC_TYPE_MPEG2                      -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MINFO_MKV_V_CODEC_TYPE_UNKNOWN = 0,
    MINFO_MKV_V_CODEC_TYPE_MS_VFW_FOURCC,
    MINFO_MKV_V_CODEC_TYPE_MPEG4_ISO_AVC,
    MINFO_MKV_V_CODEC_TYPE_MPEG4_ISO_SP,
    MINFO_MKV_V_CODEC_TYPE_MPEG4_ISO_ASP,
    MINFO_MKV_V_CODEC_TYPE_MPEG4_MS_V3,
    MINFO_MKV_V_CODEC_TYPE_MPEG1,
    MINFO_MKV_V_CODEC_TYPE_MPEG2,
    MINFO_MKV_V_CODEC_TYPE_VP8
}MINFO_MKV_VIDEO_CODEC_TYPE_T;

/* This structure define attributes of video stream in MKV container*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_MKV_VIDEO_STM_ATRBT_T
 *  @brief Mkv audio stream infor.
 *  @code
 *  typedef struct _MINFO_MKV_VIDEO_STM_ATRBT_T
 *  {
 *      MINFO_INFO_VIDEO_ENC_T          e_enc;
 *      MINFO_MKV_VIDEO_CODEC_TYPE_T    e_codec_type;
 *      UINT32                          ui4_scale;
 *      UINT32                          ui4_rate;
 *      MINFO_MKV_TM_SCALE_FLOAT_T      t_track_timecode_scale;
 *      INT32                           i4_width;
 *      INT32                           i4_height;
 *      BOOL                            b_default;
 *      UINT32                          ui4_codec_info_size;
 *      UINT8*                          pui1_codec_info;
 *  }MINFO_MKV_VIDEO_STM_ATRBT_T;
 *  @endcode
 *  @li@c  e_enc                                             -
 *  @li@c  e_codec_type                                      -
 *  @li@c  ui4_scale                                         -
 *  @li@c  ui4_rate                                          -
 *  @li@c  t_track_timecode_scale                            -
 *  @li@c  i4_width                                          -
 *  @li@c  i4_height                                         -
 *  @li@c  b_default                                         -
 *  @li@c  ui4_codec_info_size                               -
 *  @li@c  pui1_codec_info                                   -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_MKV_VIDEO_STM_ATRBT_T
{
    MINFO_INFO_VIDEO_ENC_T          e_enc;
    MINFO_MKV_VIDEO_CODEC_TYPE_T    e_codec_type;
    UINT32                          ui4_scale;
    UINT32                          ui4_rate;
    MINFO_MKV_TM_SCALE_FLOAT_T      t_track_timecode_scale;
    /*UINT32                        ui4_sample_size;*/
    INT32                           i4_width;
    INT32                           i4_height;
    INT32                           i4_dsp_width;
    INT32                           i4_dsp_height;
    /*UINT32                        ui4_length;*/
    /*UINT64                        ui8_duration;*/
    BOOL                            b_default;
#if 1/*MKV DivXPlus*/
    BOOL                            b_divx_plus;
#endif
    UINT32                          ui4_codec_info_size;
    UINT8*                          pui1_codec_info;

    /* add for content encodings */
    BOOL                            b_has_decode_setting;
    UINT32                          ui4_encoding_num;
    MINFO_MKV_CONTENT_ENCODING_T*   pt_content_encoding;
}MINFO_MKV_VIDEO_STM_ATRBT_T;

/*------------------------------------------------------------------*/
/*! @struct MINFO_MKV_SUBTITLE_STM_ATRBT_T
 *  @brief Mkv subtitle stream attribute.
 *  @code
 *  typedef struct _MINFO_MKV_SUBTITLE_STM_ATRBT_T
 *  {
 *      MINFO_INFO_SUBTITLE_TYPE_T   e_subtitle_type;
 *      UINT32                       ui4_scale;
 *      UINT32                       ui4_rate;
 *      MINFO_MKV_TM_SCALE_FLOAT_T   t_track_timecode_scale;
 *      UINT64                       ui8_attach_link_id;
 *      INT32                        i4_width;
 *      CHAR                         s_lang[ISO_639_LANG_LEN];
 *      BOOL                         b_default;
 *      UINT32                       ui4_codec_info_size;
 *      UINT8*                       pui1_codec_info;
 *      UINT32                       ui4_content_encodings_size;
 *      UINT8*                       pui1_content_encodings;
 *      UINT32                        ui4_encoding_num;
 *      MINFO_MKV_CONTENT_ENCODING_T* pt_content_encoding;
 *      MINFO_MKV_CONTENT_ENCODING_T* pt_curr_content_encoding;
 *  }MINFO_MKV_SUBTITLE_STM_ATRBT_T;
 *  @endcode
 *  @li@c  e_subtitle_type                                   -
 *  @li@c  ui4_scale                                         -
 *  @li@c  ui4_rate                                          -
 *  @li@c  t_track_timecode_scale                            -
 *  @li@c  ui8_attach_link_id                                -
 *  @li@c  i4_width                                          -
 *  @li@c  s_lang[ISO_639_LANG_LEN]                          -
 *  @li@c  b_default                                         -
 *  @li@c  ui4_codec_info_size                               -
 *  @li@c  pui1_codec_info                                   -
 *  @li@c  ui4_content_encodings_size                        -
 *  @li@c  pui1_content_encodings                            -
 *  @li@c  ui4_encoding_num                                  -
 *  @li@c  pt_content_encoding                               -
 *  @li@c  pt_curr_content_encoding                          -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_MKV_SUBTITLE_STM_ATRBT_T
{
    MINFO_INFO_SUBTITLE_TYPE_T   e_subtitle_type;
    UINT32                       ui4_scale;
    UINT32                       ui4_rate;
    MINFO_MKV_TM_SCALE_FLOAT_T   t_track_timecode_scale;
    /*add for font info*/
    UINT64                       ui8_attach_link_id;
    /*UINT32                     ui4_sample_size;
    INT32                        i4_width;
    INT32                        i4_height;*/
    CHAR                         s_lang[ISO_639_LANG_LEN];
    BOOL                         b_default;
    UINT32                       ui4_codec_info_size;
    UINT8*                       pui1_codec_info;
    UINT32                       ui4_content_encodings_size;
    UINT8*                       pui1_content_encodings;
    UINT32                        ui4_encoding_num;
    MINFO_MKV_CONTENT_ENCODING_T* pt_content_encoding;
    MINFO_MKV_CONTENT_ENCODING_T* pt_curr_content_encoding;
}MINFO_MKV_SUBTITLE_STM_ATRBT_T;

/*add for font info*/
/*------------------------------------------------------------------*/
/*! @struct FONT_INFO_T
 *  @brief Font information.
 *  @code
 *  typedef struct _FONT_INFO_T
 *  {
 *      UINT16 ac_file_name[256];
 *      CHAR   ac_file_mimetype[64];
 *      UINT64 ui8_file_offset;
 *      VOID*  pv_file_data;
 *      UINT32 ui4_font_file_size;
 *      UINT64 ui8_font_id;
 *      struct _FONT_INFO_T* pt_next_font;
 *  } FONT_INFO_T;
 *  @endcode
 *  @li@c  ac_file_name[256]                                 -
 *  @li@c  ac_file_mimetype[64]                              -
 *  @li@c  ui8_file_offset                                   -
 *  @li@c  pv_file_data                                      -
 *  @li@c  ui4_font_file_size                                -
 *  @li@c  ui8_font_id                                       -
 *  @li@c  pt_next_font                                      -
 */
/*------------------------------------------------------------------*/
typedef struct _FONT_INFO_T
{
    /*UINT16 aui2_file_description[256];*/
    UINT16 ac_file_name[256];
    CHAR   ac_file_mimetype[64];
    UINT64 ui8_file_offset;
    VOID*  pv_file_data;
    UINT32 ui4_font_file_size;
    UINT64 ui8_font_id;
    struct _FONT_INFO_T* pt_next_font;
} FONT_INFO_T;

/* This structure define attributes of audio stream in PS*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_PS_AUDIO_STM_ATRBT_T
 *  @brief Ps audio stream attribute.
 *  @code
 *  typedef struct _MINFO_PS_AUDIO_STM_ATRBT_T
 *  {
 *      MINFO_INFO_AUDIO_ENC_T      e_enc;
 *      MINFO_INFO_AUD_LAYER_T      e_layer;
 *      UINT8                       ui1_sub_stream_id;
 *      INT32                       i4_samples_per_quant;
 *      INT32                       i4_sampling_frequency;
 *      INT16                       i2_channels;
 *      BOOL                        b_default;
 *  }MINFO_PS_AUDIO_STM_ATRBT_T;
 *  @endcode
 *  @li@c  e_enc                                             -
 *  @li@c  e_layer                                           -
 *  @li@c  ui1_sub_stream_id                                 -
 *  @li@c  i4_samples_per_quant                              -
 *  @li@c  i4_sampling_frequency                             -
 *  @li@c  i2_channels                                       -
 *  @li@c  b_default                                         -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_PS_AUDIO_STM_ATRBT_T
{
    MINFO_INFO_AUDIO_ENC_T      e_enc;
    MINFO_INFO_AUD_LAYER_T      e_layer;
    UINT8                       ui1_sub_stream_id;
    INT32                       i4_samples_per_quant;
    INT32                       i4_sampling_frequency;
    INT16                       i2_channels;
    BOOL                        b_default;
    CHAR                        s_lang[ISO_639_LANG_LEN];
}MINFO_PS_AUDIO_STM_ATRBT_T;

/* This structure define attributes of video stream in PS*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_PS_VIDEO_STM_ATRBT_T
 *  @brief Ps audio stream attribute.
 *  @code
 *  typedef struct _MINFO_PS_VIDEO_STM_ATRBT_T
 *  {
 *      UINT8                       ui1_stream_id;
 *      MINFO_INFO_VIDEO_ENC_T      e_enc;
 *      MINFO_INFO_VID_FMT_T        e_vid_fmt;
 *      INT32                       i4_width;
 *      INT32                       i4_height;
 *      BOOL                        b_default;
 *  }MINFO_PS_VIDEO_STM_ATRBT_T;
 *  @endcode
 *  @li@c  ui1_stream_id                                     -
 *  @li@c  e_enc                                             -
 *  @li@c  e_vid_fmt                                         -
 *  @li@c  i4_width                                          -
 *  @li@c  i4_height                                         -
 *  @li@c  b_default                                         -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_PS_VIDEO_STM_ATRBT_T
{
    UINT8                       ui1_stream_id;
    MINFO_INFO_VIDEO_ENC_T      e_enc;
    MINFO_INFO_VID_FMT_T        e_vid_fmt;
    INT32                       i4_width;
    INT32                       i4_height;
    BOOL                        b_default;
}MINFO_PS_VIDEO_STM_ATRBT_T;

/* This structure define attributes of subtitle stream in PS*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_PS_SUBTITLE_STM_ATRBT_T
 *  @brief Ps subtitle stream attribute.
 *  @code
 *  typedef struct _MINFO_PS_SUBTITLE_STM_ATRBT_T
 *  {
 *      UINT32                      ui4_reserved;
 *  }MINFO_PS_SUBTITLE_STM_ATRBT_T;
 *  @endcode
 *  @li@c  ui4_reserved                                      -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_PS_SUBTITLE_STM_ATRBT_T
{
    UINT8                     ui1_strm_id;
    UINT8                     ui1_sub_strm_id; 
}MINFO_PS_SUBTITLE_STM_ATRBT_T;



/* This structure define attributes of external subtitle stream*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_EXT_SUBTITLE_STM_ATRBT_T
 *  @brief Ps external subtitle stream attribute.
 *  @code
 *  typedef struct _MINFO_EXT_SUBTITLE_STM_ATRBT_T
 *  {
 *       HANDLE_T                                                 h_ext_subtitle;
 *     CHAR                                     c_lang_list[4];
 *         HANDLE_T                            h_related_subtitle;
 *     UINT16                                ui2_idx;
 *     BOOL                                b_is_related;
 *  }MINFO_EXT_SUBTITLE_STM_ATRBT_T;
 *  @endcode
 *  @li@c  ui4_reserved                                      -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_EXT_SUBTITLE_STM_ATRBT_T
{
    HANDLE_T        h_ext_subtitle;
    CHAR            c_lang_list[4];
    HANDLE_T        h_related_subtitle;
    UINT16          ui2_idx;
    BOOL            b_is_related;
}MINFO_EXT_SUBTITLE_STM_ATRBT_T;


typedef struct 
{
    MINFO_INFO_VIDEO_ENC_T      e_enc;
    UINT32                      ui4_stream_id;
}MINFO_ES_VIDEO_STM_ATRBT_T;

/*------------------------------------------------------------------*/
/*! @struct MINFO_ES_AUDIO_STM_ATRBT_T
 *  @brief Es audio stream attribute.
 *  @code
 *  typedef struct _MMINFO_ES_AUDIO_STM_ATRBT_T
 *  {
 *      MINFO_INFO_AUDIO_ENC_T      e_enc;
 *      MINFO_INFO_AUD_LAYER_T      e_layer;
 *      UINT32                      ui4_sampling_frequency;
 *      UINT16                      ui2_bits_per_sample;
 *      UINT16                      ui2_channels;
 *      BOOL                        b_big_endian;
 *  }MINFO_ES_AUDIO_STM_ATRBT_T;
 *  @endcode
 *  @li@c  e_enc                                             -
 *  @li@c  e_layer                                           -
 *  @li@c  ui4_sampling_frequency                            -
 *  @li@c  ui2_bits_per_sample                               -
 *  @li@c  ui2_channels                                      -
 *  @li@c  b_big_endian                                      -
 */
/*------------------------------------------------------------------*/
typedef struct _MMINFO_ES_AUDIO_STM_ATRBT_T
{
    MINFO_INFO_AUDIO_ENC_T      e_enc;
    MINFO_INFO_AUD_LAYER_T      e_layer;
    UINT32                      ui4_sampling_frequency;
    UINT16                      ui2_bits_per_sample;
    UINT16                      ui2_channels;
    BOOL                        b_big_endian;
    BOOL                        b_signed;
    CHAR                        s_lang[ISO_639_LANG_LEN];
}MINFO_ES_AUDIO_STM_ATRBT_T;

/*ogg strm info*/
typedef enum
{
    OGG_STRM_TYPE_UNKNOWN=0,
    OGG_STRM_TYPE_VORBIS,
    OGG_STRM_TYPE_AUDIO,
    OGG_STRM_TYPE_VIDEO,
    OGG_STRM_TYPE_TEXT
}OGG_STRM_TYPE_T;

typedef struct _OGG_STREAM_VORBIS_HEADER_T
{
    MINFO_INFO_AUDIO_ENC_T      e_enc;
    UINT8       ui1_vorbis_id[7];        /*0x01+"vorbis"*/
    UINT32      ui4_vorbis_version;
    UINT8       ui1_audio_channel;
    UINT32      ui4_audio_sample_rate;
    UINT32      ui4_bitrate_maximun;
    UINT32      ui4_bitrate_norminal;
    UINT32      ui4_bitrate_minimun;
    UINT8       blocksize;
    UINT8       frameflag;
}OGG_STREAM_VORBIS_INFO_T;

typedef struct _OGG_STREAM_AUDIO_T
{
    MINFO_INFO_AUDIO_ENC_T      e_enc;
    MINFO_INFO_AUD_LAYER_T      e_layer;
    UINT8       ui1_audio_id[9];            /*0x01+"audio"*/
    UINT32      ui4_sub_type;
    UINT32      ui4_size;
    UINT64      ui8_timeunit;
    UINT64      ui8_sample_per_unit;
    UINT32      ui4_default_len;
    UINT32      ui4_buffer_size;
    UINT16      ui2_bit_per_sample;
    UINT16      ui2_padding;
    UINT16      ui2_channels;
    UINT16      ui2_blockalign;
    UINT32      ui4_avg_bytes_per_sec;
}OGG_STREAM_AUDIO_INFO_T;

typedef struct _MINFO_OGG_VIDEO_STM_ATRBT_T
{
    MINFO_INFO_VIDEO_ENC_T        e_enc;
    UINT32      ui4_serial_num;          /* stream serial num*/
    UINT32      ui4_video_page_num;          
    UINT32      ui4_avg_packet_size;         /*avg page length*/
    UINT32      ui4_avg_frame_num;         /*avg granule pos*/
    UINT64      ui8_granule_pos;           /*eos granule_position*/
    UINT8       ui1_video_id[9];            /*0x01+"video"*/
    UINT32      ui4_sub_type;
    UINT32      ui4_size;
    UINT64      ui8_timeunit;
    UINT64      ui8_sample_per_unit;
    UINT32      ui4_default_len;
    UINT32      ui4_buffer_size;
    UINT16      ui2_bit_per_sample;
    UINT16      ui2_padding;
    UINT32      ui4_width;
    UINT32      ui4_height;    
}MINFO_OGG_VIDEO_STM_ATRBT_T;

typedef struct _MINFO_OGG_TEXT_STM_ATRBT_T
{
    UINT32       ui4_reserved;
}MINFO_OGG_TEXT_STM_ATRBT_T;

typedef struct _MINFO_OGG_AUDIO_VORBIS_SPEC_T
{
    UINT32      vorbis_header_offset;
    UINT32      setup_offset;
    UINT32      comment_offset;
}MINFO_OGG_AUDIO_VORBIS_SPEC_T;

typedef struct _MINFO_OGG_AUDIO_STM_ATRBT_T
{
    OGG_STRM_TYPE_T     t_ogg_audio_type;
    UINT32              ui4_serial_num;        /* stream serial num*/
    UINT64              ui8_granule_pos;       /*eos granule_position*/
    UINT32              ui4_audio_page_num; /*eos page sequence num*/
    UINT32              ui4_avg_frame_num;         /*avg granule pos*/
    UINT32              ui4_avg_packet_size;         /*avg page length*/
    union
    {
        OGG_STREAM_VORBIS_INFO_T    t_ogg_strm_vorbis_info;
        OGG_STREAM_AUDIO_INFO_T     t_ogg_strm_audio_info;
    }u;
    MINFO_OGG_AUDIO_VORBIS_SPEC_T   t_ogg_vorbis_spec;
}MINFO_OGG_AUDIO_STM_ATRBT_T;

/*------------------ TS file AV stream attribute ---------------------*/
typedef enum
{
    MINFO_AUD_TYPE_UNKNOWN =0,
    MINFO_AUD_TYPE_CLEAN,
    MINFO_AUD_TYPE_HEARING_IMPAIRED,
    MINFO_AUD_TYPE_VISUAL_IMPAIRED,
    MINFO_AUD_TYPE_RESERVED
}MINFO_AUDIO_TYPE_T;

typedef enum
{
    MINFO_AUD_PROF_LVL_RESERVED    = 0,
    MINFO_AUD_PROF_LVL_AAC_LV4     = 0x52,
    MINFO_AUD_PROF_LVL_HEAAC_LVL2  = 0x58,
    MINFO_AUD_PROF_LVL_HEAAC_LVL4  = 0x5A    
}MINFO_AUDIO_PROFILE_LVL_T;

typedef enum
{
    MINFO_AUD_MIX_TYPE_UNKNOWN    = 0,  
    MINFO_AUD_MIX_TYPE_SUPPLEMENTARY,
    MINFO_AUD_MIX_TYPE_INDEPENDENT
}MINFO_AUDIO_MIX_TYPE_T;

typedef enum
{
    MINFO_AUD_CLASSIFIC_TYPE_RESERVED  = 0,  
    MINFO_AUD_CLASSIFIC_TYPE_MAIN,  
    MINFO_AUD_CLASSIFIC_TYPE_VISUAL_IMPAIRED_AD,  
    MINFO_AUD_CLASSIFIC_TYPE_HEARING_IMPAIRED_CLEAN,  
    MINFO_AUD_CLASSIFIC_TYPE_VISUAL_IMPAIRED_SPOKEN_SUBTITLE,  
}MINFO_AUDIO_CLASSIFIC_TYPE_T;

typedef enum
{
    MINFO_TS_SBTL_TYPE_UNKNOWN    = 0, 
    MINFO_TS_SBTL_TYPE_NO_ASP_RATIO,
    MINFO_TS_SBTL_TYPE_4_3,
    MINFO_TS_SBTL_TYPE_16_9,
    MINFO_TS_SBTL_TYPE_221_1,
    MINFO_TS_SBTL_TYPE_HD,
    /* Aspect ratio for Hard-Of-Hearing. */
    MINFO_TS_SBTL_TYPE_NO_ASP_RATIO_HOH,
    MINFO_TS_SBTL_TYPE_4_3_HOH,
    MINFO_TS_SBTL_TYPE_16_9_HOH,
    MINFO_TS_SBTL_TYPE_221_1_HOH,
    MINFO_TS_SBTL_TYPE_HD_HOH 
}MINFO_TS_SBTL_TYPE_T;

/* for 3D video */
typedef enum
{
    MINFO_TS_REALD_LAYER_TYPE_UNKNOWN    = 0, 
    MINFO_TS_REALD_LAYER_TYPE_BASE, 
    MINFO_TS_REALD_LAYER_TYPE_ENHANCEMENT
}MINFO_TS_REALD_LAYER_TYPE_T;

typedef enum
{
    MINFO_TS_REALD_CODING_METHOR_CRITICALLY_SAMPLED    = 0, 
    MINFO_TS_REALD_CODING_METHOR_NON_CRITICALLY_SAMPLED= 0x80
}MINFO_TS_REALD_CODING_METHOD_T;


/* RD3D_registration descriptor */
typedef struct 
{
    BOOL                            b_rd3d;
    MINFO_TS_REALD_LAYER_TYPE_T     e_layer_type;
    MINFO_TS_REALD_CODING_METHOD_T  e_coding_method;    
} MINFO_TS_RD3D_REG_DESC_T;

/* MVC extension descriptor */
typedef struct 
{
    BOOL        b_mvc_video;
    BOOL        b_mvc_ext_desc;
    UINT16      ui2_avg_bit_rate;
    UINT16      ui2_max_bit_rate;
    UINT16      ui2_view_ord_idx_min;
    UINT16      ui2_view_ord_idx_max;
    UINT8       ui1_temporal_id_start;
    UINT8       ui1_temporal_id_end;
    BOOL        b_no_sei_nal_present;
    BOOL        b_no_prefix_nal_present;
} MINFO_MVC_EXTENSION_DESC_T;

typedef struct  _MINFO_TS_VIDEO_STRM_ATRBT_T
{
    MPEG_2_PID_T                ui2_pid;
    MINFO_INFO_VIDEO_ENC_T      e_enc;
    MINFO_INFO_VID_FMT_T        e_vid_fmt;
    BOOL                        b_default;    
    UINT8                       ui1_level_idc;
    UINT8                       ui1_component_tag;
    UINT16                      ui2_pmt_index;
    MINFO_TS_RD3D_REG_DESC_T    t_rd3d;
    MINFO_MVC_EXTENSION_DESC_T  t_mvc_ext;
    UINT32                      ui4_width;
    UINT32                      ui4_height;
} MINFO_TS_VIDEO_STM_ATRBT_T;

typedef struct _MINFO_TS_AUDIO_STRM_ATRBT_T
{
    MPEG_2_PID_T                    ui2_pid;
    MINFO_INFO_AUDIO_ENC_T          e_enc;
    MINFO_INFO_AUD_LAYER_T          e_layer;
    MINFO_AUDIO_TYPE_T              e_type;
    MINFO_AUDIO_PROFILE_LVL_T       e_prof_lvl;
    UINT8                           ui1_index;
    UINT16                          ui2_pmt_index;

    /* supplementary descriptor support*/
    MINFO_AUDIO_MIX_TYPE_T          e_mix_type;
    MINFO_AUDIO_CLASSIFIC_TYPE_T    e_class;

    UINT8                           ui1_component_tag;
    CHAR                            s_lang[ISO_639_LANG_LEN];
    BOOL                            b_default;

    /* The following infor only can be got when it is playing*/
    UINT32                          ui2_channels;
    UINT32                          ui4_sample_rate;
    UINT32                          ui4_data_rate;
    UINT8                           ui1_bit_depth;
} MINFO_TS_AUDIO_STM_ATRBT_T;

typedef struct 
{
    MPEG_2_PID_T               ui2_pid;
    UINT16                     ui2_comp_pg_id;
    UINT16                     ui2_anci_pg_id;
    CHAR                       s_lang [ISO_639_LANG_LEN];
    MINFO_TS_SBTL_TYPE_T       e_sbtl_type;
    UINT16                     ui2_pmt_index;
}  MINFO_TS_SBTL_STM_ATRBT_T;
/* This structure define stream id which  media info utility support*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_STM_ID_T
 *  @brief Stream ID..
 *  @code
 *  typedef struct _MNIFO_STM_ID_T
 *  {
 *      union
 *      {
 *         MINFO_AVI_STM_ID_T           t_avi_stm_id;
 *         MINFO_ASF_STM_ID_T           t_asf_stm_id;
 *         MINFO_MP4_STM_ID_T           t_mp4_stm_id;
 *         MINFO_MKV_STM_ID_T           t_mkv_stm_id;
 *         MINFO_MPEG_2_STREAM_ID       t_ps_stm_id;
 *         MINFO_RM_STM_ID_T            t_rm_stm_id;
 *         MINFO_AIF_STM_ID_T           t_aif_stm_id;
 *      }u;
 *  }MINFO_STM_ID_T;
 *  @endcode
 *  @li@c  t_avi_stm_id                                      -
 *  @li@c  t_asf_stm_id                                      -
 *  @li@c  t_mp4_stm_id                                      -
 *  @li@c  t_mkv_stm_id                                      -
 *  @li@c  t_ps_stm_id                                       -
 *  @li@c  t_rm_stm_id                                       -
 *  @li@c  t_aif_stm_id                                      -
 */
/*------------------------------------------------------------------*/
typedef struct _MNIFO_STM_ID_T
{
    union
    {
       MINFO_AVI_STM_ID_T           t_avi_stm_id;
       MINFO_ASF_STM_ID_T           t_asf_stm_id;
       MINFO_MP4_STM_ID_T           t_mp4_stm_id;
       MINFO_MKV_STM_ID_T           t_mkv_stm_id;
       MINFO_MPEG_2_STREAM_ID       t_ps_stm_id;
       MINFO_RM_STM_ID_T            t_rm_stm_id;
       MINFO_AIF_STM_ID_T           t_aif_stm_id;
       MINFO_FLV_STM_ID_T           t_flv_stm_id;
       MINFO_OGG_STM_ID_T        t_ogg_stm_id;
    }u;
}MINFO_STM_ID_T;


/* This structure define stream info which  media info utility support*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_TYPE_STM_ATRBT_T
 *  @brief Stream attribute of every stream.
 *  @code
 *  typedef struct _MINFO_TYPE_STM_ATRBT_T
 *  {
 *      MINFO_STM_ID_T       t_stm_id;
 *      BOOL                 b_is_codec_support;
 *      union
 *      {
 *        MINFO_AVI_AUDIO_STM_ATRBT_T                   t_avi_audio_stm_attr;
 *        MINFO_AVI_VIDEO_STM_ATRBT_T                   t_avi_video_stm_attr;
 *        MINFO_AVI_DIVX_SUBTITLE_STM_ATRBT_T           t_avi_sp_stm_attr;
 *        MINFO_ASF_AUDIO_STM_ATRBT_T                   t_asf_audio_stm_attr;
 *        MINFO_ASF_VIDEO_STM_ATRBT_T                   t_asf_video_stm_attr;
 *        MINFO_MP4_AUDIO_STM_ATRBT_T                   t_mp4i_audio_stm_attr;
 *        MINFO_MP4_VIDEO_STM_ATRBT_T                   t_mp4i_video_stm_attr;
 *        MINFO_MP4_SUBTITLE_STM_ATRBT_T                t_mp4_sp_stm_attr;
 *        MINFO_MKV_AUDIO_STM_ATRBT_T                   t_mkv_audio_stm_attr;
 *        MINFO_MKV_VIDEO_STM_ATRBT_T                   t_mkv_video_stm_attr;
 *        MINFO_MKV_SUBTITLE_STM_ATRBT_T                t_mkv_sp_stm_attr;
 *        MINFO_PS_AUDIO_STM_ATRBT_T                    t_ps_audio_stm_attr;
 *        MINFO_PS_VIDEO_STM_ATRBT_T                    t_ps_video_stm_attr;
 *        MINFO_PS_SUBTITLE_STM_ATRBT_T                 t_ps_sp_stm_attr;
 *        MINFO_EXT_SUBTITLE_STM_ATRBT_T                t_ex_subtt_stm_attr;
 *        MINFO_MP3_AUDIO_STM_ATRBT_T                   t_mp3_audio_stm_attr;
 *        MINFO_ES_AUDIO_STM_ATRBT_T                    t_es_audio_stm_attr;
 *        MINFO_RM_VIDEO_STM_ATRBT_T                    t_rm_video_stm_attr;
 *        MINFO_RM_AUDIO_STM_ATRBT_T                    t_rm_audio_stm_attr;
 *        MINFO_AIF_AUDIO_STM_ATRBT_T                   t_aif_audio_stm_attr;
 *      }u;
 *  }MINFO_TYPE_STM_ATRBT_T;
 *  @endcode
 *  @li@c  t_stm_id                                          -
 *  @li@c  b_is_codec_support                                -
 *  @li@c  t_avi_audio_stm_attr                              -
 *  @li@c  t_avi_video_stm_attr                              -
 *  @li@c  t_avi_sp_stm_attr                                 -
 *  @li@c  t_asf_audio_stm_attr                              -
 *  @li@c  t_asf_video_stm_attr                              -
 *  @li@c  t_mp4i_audio_stm_attr                             -
 *  @li@c  t_mp4i_video_stm_attr                             -
 *  @li@c  t_mp4_sp_stm_attr                                 -
 *  @li@c  t_mkv_audio_stm_attr                              -
 *  @li@c  t_mkv_video_stm_attr                              -
 *  @li@c  t_mkv_sp_stm_attr                                 -
 *  @li@c  t_ps_audio_stm_attr                               -
 *  @li@c  t_ps_video_stm_attr                               -
 *  @li@c  t_ps_sp_stm_attr                                  -
 *  @li@c  t_ex_subtt_stm_attr                               -
 *  @li@c  t_mp3_audio_stm_attr                              -
 *  @li@c  t_es_audio_stm_attr                               -
 *  @li@c  t_rm_video_stm_attr                               -
 *  @li@c  t_rm_audio_stm_attr                               -
 *  @li@c  t_aif_audio_stm_attr                              -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_TYPE_STM_ATRBT_T
{
    MINFO_STM_ID_T       t_stm_id;
    BOOL                 b_is_codec_support;
    union
    {
      MINFO_AVI_AUDIO_STM_ATRBT_T                   t_avi_audio_stm_attr;
      MINFO_AVI_VIDEO_STM_ATRBT_T                   t_avi_video_stm_attr;
      MINFO_AVI_DIVX_SUBTITLE_STM_ATRBT_T           t_avi_sp_stm_attr;
      MINFO_ASF_AUDIO_STM_ATRBT_T                   t_asf_audio_stm_attr;
      MINFO_ASF_VIDEO_STM_ATRBT_T                   t_asf_video_stm_attr;
      MINFO_MP4_AUDIO_STM_ATRBT_T                   t_mp4i_audio_stm_attr;
      MINFO_MP4_VIDEO_STM_ATRBT_T                   t_mp4i_video_stm_attr;
      MINFO_MP4_SUBTITLE_STM_ATRBT_T                t_mp4_sp_stm_attr;
      MINFO_MKV_AUDIO_STM_ATRBT_T                   t_mkv_audio_stm_attr;
      MINFO_MKV_VIDEO_STM_ATRBT_T                   t_mkv_video_stm_attr;
      MINFO_MKV_SUBTITLE_STM_ATRBT_T                t_mkv_sp_stm_attr;
      MINFO_PS_AUDIO_STM_ATRBT_T                    t_ps_audio_stm_attr;
      MINFO_PS_VIDEO_STM_ATRBT_T                    t_ps_video_stm_attr;
      MINFO_PS_SUBTITLE_STM_ATRBT_T                 t_ps_sp_stm_attr;
      MINFO_EXT_SUBTITLE_STM_ATRBT_T                t_ex_subtt_stm_attr;
      MINFO_MP3_AUDIO_STM_ATRBT_T                   t_mp3_audio_stm_attr;
      MINFO_ES_VIDEO_STM_ATRBT_T                    t_es_video_stm_attr;
      MINFO_ES_AUDIO_STM_ATRBT_T                    t_es_audio_stm_attr;
      MINFO_RM_VIDEO_STM_ATRBT_T                    t_rm_video_stm_attr;
      MINFO_RM_AUDIO_STM_ATRBT_T                    t_rm_audio_stm_attr;
      MINFO_AIF_AUDIO_STM_ATRBT_T                   t_aif_audio_stm_attr;
      /*add for flv*/
      MINFO_FLV_AUDIO_STM_ATRBT_T                   t_flv_audio_stm_attr;
      MINFO_FLV_VIDEO_STM_ATRBT_T                   t_flv_video_stm_attr;
      
      MINFO_CMPB_VIDEO_STM_ATRBT_T                  t_cmpb_video_stm_attr;
      MINFO_CMPB_AUDIO_STM_ATRBT_T                  t_cmpb_audio_stm_attr;
      MINFO_CMPB_SUBTL_STM_ATRBT_T                  t_cmpb_subtl_stm_attr;

      MINFO_OGG_VIDEO_STM_ATRBT_T                    t_ogg_video_stm_attr;
      MINFO_OGG_AUDIO_STM_ATRBT_T                    t_ogg_audio_stm_attr;

      /* mutil not implement, just for output infor from scdb */
      MINFO_TS_AUDIO_STM_ATRBT_T                    t_ts_audio_stm_attr;
      MINFO_TS_VIDEO_STM_ATRBT_T                    t_ts_video_stm_attr;
      MINFO_TS_SBTL_STM_ATRBT_T                     t_ts_sp_stm_attr;
    }u;
}MINFO_TYPE_STM_ATRBT_T;



/* This structure enumerates all ID3 pic type of media info.  */
/*------------------------------------------------------------------*/
/*! @enum MINFO_ID3_PIC_TYPE_T
 *  @brief ID3 pic type.
 *  @code
 *  typedef enum
 *  {
 *      MINFO_ID3_PIC_OTHER = 0,      
 *      MINFO_ID3_PIC_32X32_ICON,     
 *      MINFO_ID3_PIC_OTHER_ICON,     
 *      MINFO_ID3_PIC_COVER_FRONT,    
 *      MINFO_ID3_PIC_COVER_BACK,     
 *      MINFO_ID3_PIC_LEAFLET,        
 *      MINFO_ID3_PIC_MEDIA,          
 *      MINFO_ID3_PIC_LEAD,           
 *      MINFO_ID3_PIC_ARTIST,         
 *      MINFO_ID3_PIC_CONDUCTOR,      
 *      MINFO_ID3_PIC_BAND,           
 *      MINFO_ID3_PIC_COMPOSER,       
 *      MINFO_ID3_PIC_LYRICIST,       
 *      MINFO_ID3_PIC_RECORD_LOC,     
 *      MINFO_ID3_PIC_DUR_RECORD,     
 *      MINFO_ID3_PIC_DUR_PERFPRM,    
 *      MINFO_ID3_PIC_SCREEN_CAPTURE, 
 *      MINFO_ID3_PIC_COLOUR_FISH,    
 *      MINFO_ID3_PIC_ILLUSTRATION,   
 *      MINFO_ID3_PIC_BAND_LOGO_TYPE, 
 *      MINFO_ID3_PIC_STUDIO_LOGOTYPE 
 *  } MINFO_ID3_PIC_TYPE_T;
 *  @endcode
 *  @li@c  MINFO_ID3_PIC_OTHER                               -
 *  @li@c  MINFO_ID3_PIC_32X32_ICON                          -
 *  @li@c  MINFO_ID3_PIC_OTHER_ICON                          -
 *  @li@c  MINFO_ID3_PIC_COVER_FRONT                         -
 *  @li@c  MINFO_ID3_PIC_COVER_BACK                          -
 *  @li@c  MINFO_ID3_PIC_LEAFLET                             -
 *  @li@c  MINFO_ID3_PIC_MEDIA                               -
 *  @li@c  MINFO_ID3_PIC_LEAD                                -
 *  @li@c  MINFO_ID3_PIC_ARTIST                              -
 *  @li@c  MINFO_ID3_PIC_CONDUCTOR                           -
 *  @li@c  MINFO_ID3_PIC_BAND                                -
 *  @li@c  MINFO_ID3_PIC_COMPOSER                            -
 *  @li@c  MINFO_ID3_PIC_LYRICIST                            -
 *  @li@c  MINFO_ID3_PIC_RECORD_LOC                          -
 *  @li@c  MINFO_ID3_PIC_DUR_RECORD                          -
 *  @li@c  MINFO_ID3_PIC_DUR_PERFPRM                         -
 *  @li@c  MINFO_ID3_PIC_SCREEN_CAPTURE                      -
 *  @li@c  MINFO_ID3_PIC_COLOUR_FISH                         -
 *  @li@c  MINFO_ID3_PIC_ILLUSTRATION                        -
 *  @li@c  MINFO_ID3_PIC_BAND_LOGO_TYPE                      -
 *  @li@c  MINFO_ID3_PIC_STUDIO_LOGOTYPE                     -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MINFO_ID3_PIC_OTHER = 0,      /* Other */
    MINFO_ID3_PIC_32X32_ICON,     /* 32x32 pixels 'file icon' (PNG only) */
    MINFO_ID3_PIC_OTHER_ICON,     /* Other file icon */
    MINFO_ID3_PIC_COVER_FRONT,    /* Cover (front) */
    MINFO_ID3_PIC_COVER_BACK,     /* Cover (back) */
    MINFO_ID3_PIC_LEAFLET,        /* Leaflet page */
    MINFO_ID3_PIC_MEDIA,          /* Media (e.g. lable side of CD) */
    MINFO_ID3_PIC_LEAD,           /* Lead artist/lead performer/soloist */
    MINFO_ID3_PIC_ARTIST,         /* Artist/performer */
    MINFO_ID3_PIC_CONDUCTOR,      /* Conductor */
    MINFO_ID3_PIC_BAND,           /* Band/Orchestra */
    MINFO_ID3_PIC_COMPOSER,       /* Composer */
    MINFO_ID3_PIC_LYRICIST,       /* Lyricist/text writer */
    MINFO_ID3_PIC_RECORD_LOC,     /* Recording Location */
    MINFO_ID3_PIC_DUR_RECORD,     /* During recording */
    MINFO_ID3_PIC_DUR_PERFPRM,    /* During performance */
    MINFO_ID3_PIC_SCREEN_CAPTURE, /* Movie/video screen capture */
    MINFO_ID3_PIC_COLOUR_FISH,    /* A bright coloured fish */
    MINFO_ID3_PIC_ILLUSTRATION,   /* Illustration */
    MINFO_ID3_PIC_BAND_LOGO_TYPE, /* Band/artist logotype */
    MINFO_ID3_PIC_STUDIO_LOGOTYPE /* Publisher/Studio logotype */
} MINFO_ID3_PIC_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum MINFO_INFO_ID3_TYPE
 *  @brief id3 type.
 *  @code
 *  typedef enum
 *  {
 *      MINFO_INFO_ID3_UNKNOWN   = 0,
 *      MINFO_INFO_ID3_V1,
 *      MINFO_INFO_ID3_V2
 *  }MINFO_INFO_ID3_TYPE;
 *  @endcode
 *  @li@c  MINFO_INFO_ID3_UNKNOWN                       -
 *  @li@c  MINFO_INFO_ID3_V1                            -
 *  @li@c  MINFO_INFO_ID3_V2                            -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MINFO_INFO_ID3_UNKNOWN   = 0,
    MINFO_INFO_ID3_V1,
    MINFO_INFO_ID3_V2
}MINFO_INFO_ID3_TYPE;

#define MINFO_ID3_PIC_OTHER_BITMASK             ((UINT32)(1<<MINFO_ID3_PIC_OTHER))    /**<        */
#define MINFO_ID3_PIC_32X32_ICON_BITMASK        ((UINT32)(1<<MINFO_ID3_PIC_32X32_ICON))    /**<        */
#define MINFO_ID3_PIC_OTHER_ICON_BITMASK        ((UINT32)(1<<MINFO_ID3_PIC_OTHER_ICON))    /**<        */
#define MINFO_ID3_PIC_COVER_FRONT_BITMASK       ((UINT32)(1<<MINFO_ID3_PIC_COVER_FRONT))    /**<        */
#define MINFO_ID3_PIC_COVER_BACK_BITMASK        ((UINT32)(1<<MINFO_ID3_PIC_COVER_BACK))    /**<        */
#define MINFO_ID3_PIC_LEAFLET_BITMASK           ((UINT32)(1<<MINFO_ID3_PIC_LEAFLET))    /**<        */
#define MINFO_ID3_PIC_MEDIA_BITMASK             ((UINT32)(1<<MINFO_ID3_PIC_MEDIA))    /**<        */
#define MINFO_ID3_PIC_LEAD_BITMASK              ((UINT32)(1<<MINFO_ID3_PIC_LEAD))    /**<        */
#define MINFO_ID3_PIC_ARTIST_BITMASK            ((UINT32)(1<<MINFO_ID3_PIC_ARTIST))    /**<        */
#define MINFO_ID3_PIC_CONDUCTOR_BITMASK         ((UINT32)(1<<MINFO_ID3_PIC_CONDUCTOR))    /**<        */
#define MINFO_ID3_PIC_BAND_BITMASK              ((UINT32)(1<<MINFO_ID3_PIC_BAND))    /**<        */
#define MINFO_ID3_PIC_COMPOSER_BITMASK          ((UINT32)(1<<MINFO_ID3_PIC_COMPOSER))    /**<        */
#define MINFO_ID3_PIC_LYRICIST_BITMASK          ((UINT32)(1<<MINFO_ID3_PIC_LYRICIST))    /**<        */
#define MINFO_ID3_PIC_RECORD_LOC_BITMASK        ((UINT32)(1<<MINFO_ID3_PIC_RECORD_LOC))    /**<        */
#define MINFO_ID3_PIC_DUR_RECORD_BITMASK        ((UINT32)(1<<MINFO_ID3_PIC_DUR_RECORD))    /**<        */
#define MINFO_ID3_PIC_DUR_PERFPRM_BITMASK       ((UINT32)(1<<MINFO_ID3_PIC_DUR_PERFPRM))    /**<        */
#define MINFO_ID3_PIC_SCREEN_CAPTURE_BITMASK    ((UINT32)(1<<MINFO_ID3_PIC_SCREEN_CAPTURE))    /**<        */
#define MINFO_ID3_PIC_COLOUR_FISH_BITMASK       ((UINT32)(1<<MINFO_ID3_PIC_COLOUR_FISH))    /**<        */
#define MINFO_ID3_PIC_ILLUSTRATION_BITMASK      ((UINT32)(1<<MINFO_ID3_PIC_ILLUSTRATION))    /**<        */
#define MINFO_ID3_PIC_STUDIO_LOGOTYPE_BITMASK   ((UINT32)(1<<MINFO_ID3_PIC_STUDIO_LOGOTYPE))    /**<        */

/*------------------------------------------------------------------*/
/*! @struct MINFO_INFO_PIC_T
 *  @brief Pic information.
 *  @code
 *  typedef struct _MINFO_INFO_PIC_T
 *  {
 *      MINFO_ID3_PIC_TYPE_T    e_pic_type;
 *      UINT32                  ui4_starting_offset;
 *      UINT32                  ui4_length;
 *      MEDIA_FORMAT_T          t_format;
 *  }MINFO_INFO_PIC_T;
 *  @endcode
 *  @li@c  e_pic_type                                        -
 *  @li@c  ui4_starting_offset                               -
 *  @li@c  ui4_length                                        -
 *  @li@c  t_format                                          -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_INFO_PIC_T
{
    MINFO_ID3_PIC_TYPE_T    e_pic_type;
    UINT32                  ui4_starting_offset;
    UINT32                  ui4_length;
    UINT32                  ui4_width;
    UINT32                  ui4_height;
    MEDIA_FORMAT_T          t_format;
}MINFO_INFO_PIC_T;

#define MINFO_MAX_ID3_PIC_NUM                     20    /**<        */

/*------------------------------------------------------------------*/
/*! @struct _MINFO_ID3_RAW_DATA_T
 *  @brief Id3 raw data information.
 *  @code
 *  typedef struct _MINFO_INFO_ID3_RAW_DATA
 *  {
 *        MINFO_INFO_ID3_TYPE     e_id3_type;
 *        UINT8*                  pui8_raw_data;
 *        SIZE_T*                 pz_data_size;
 *  }MINFO_ID3_RAW_DATA_T;
 *  @endcode
 *  @li@c  e_id3_type                                 -
 *  @li@c  pui8_raw_data                              -
 *  @li@c  pz_data_size                               -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_ID3_RAW_DATA_T
{
    MINFO_INFO_ID3_TYPE     e_id3_type;
    UINT8*                  pui1_raw_data;
    SIZE_T*                 pz_data_size; 
}MINFO_ID3_RAW_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct MINFO_TYPE_ID3_TAG_T
 *  @brief ID3 tag information.
 *  @code
 *  typedef struct _MNIFO_TYPE_ID3_TAG_T
 *  {
 *      MINFO_INFO_PIC_T    at_id3_attatched_pic[MINFO_MAX_ID3_PIC_NUM];  
 *      UINT32              ui4_pic_bitmask;
 *      SIZE_T              z_id3_year_len;
 *      UTF16_T*            pwc_str_id3_year;
 *      SIZE_T              z_id3_title_len;
 *      UTF16_T*            pwc_str_id3_title;
 *      SIZE_T              z_id3_artist_len;
 *      UTF16_T*            pwc_str_id3_artist;
 *      SIZE_T              z_id3_album_len;
 *      UTF16_T*            pwc_str_id3_album;
 *      SIZE_T              z_id3_track_len;
 *      UTF16_T*            pwc_str_id3_track;
 *      SIZE_T              z_id3_genre_len;
 *      UTF16_T*            pwc_str_id3_genre;
 *      UINT32              ui4_id3_v1_len;
 *      UINT32              ui4_id3_v2_len;
 *  }MINFO_TYPE_ID3_TAG_T;
 *  @endcode
 *  @li@c  at_id3_attatched_pic[MINFO_MAX_ID3_PIC_NUM]       -
 *  @li@c  ui4_pic_bitmask                                   -
 *  @li@c  z_id3_year_len                                    -
 *  @li@c  pwc_str_id3_year                                  -
 *  @li@c  z_id3_title_len                                   -
 *  @li@c  pwc_str_id3_title                                 -
 *  @li@c  z_id3_artist_len                                  -
 *  @li@c  pwc_str_id3_artist                                -
 *  @li@c  z_id3_album_len                                   -
 *  @li@c  pwc_str_id3_album                                 -
 *  @li@c  z_id3_track_len                                   -
 *  @li@c  pwc_str_id3_track                                 -
 *  @li@c  z_id3_genre_len                                   -
 *  @li@c  pwc_str_id3_genre                                 -
 *  @li@c  ui4_id3_v1_len                                    -
 *  @li@c  ui4_id3_v2_len                                    -
 */
/*------------------------------------------------------------------*/
typedef struct _MNIFO_TYPE_ID3_TAG_T
{
    MINFO_INFO_PIC_T    at_id3_attatched_pic[MINFO_MAX_ID3_PIC_NUM];  /*MINFO_INFO_PIC_T */
    MINFO_ID3_RAW_DATA_T t_id3_raw_data;
    UINT32              ui4_img_buf_len;
    UINT8*              pui1_img_data_buf;
    UINT32              ui4_pic_bitmask;
    SIZE_T              z_id3_year_len;
    UTF16_T*            pwc_str_id3_year;
    SIZE_T              z_id3_title_len;
    UTF16_T*            pwc_str_id3_title;
    SIZE_T              z_id3_artist_len;
    UTF16_T*            pwc_str_id3_artist;
    SIZE_T              z_id3_album_len;
    UTF16_T*            pwc_str_id3_album;
    SIZE_T              z_id3_track_len;
    UTF16_T*            pwc_str_id3_track;
    SIZE_T              z_id3_genre_len;
    UTF16_T*            pwc_str_id3_genre;
    UINT32              ui4_id3_v1_len;
    UINT32              ui4_id3_v2_len;
}MINFO_TYPE_ID3_TAG_T;

#if SUPPORT_MULTI_INFO
/*------------------------------------------------------------------*/
/*! @struct MINFO_TYPE_SCALE_INFO_T
 *  @brief Scale information.
 *  @code
 *  typedef struct _MINFO_TYPE_SCALE_INFO_T
 *  {
 *      UINT64 ui8_pb_duration;
 *      UINT64 ui8_media_sz; 
 *      UINT64 ui8_timecode_scale; 
 *      UINT64 ui8_idx_offset;
 *      UINT64 ui8_idx_sz;
 *      UINT64 ui8_data_offset;
 *      UINT64 ui8_data_sz;
 *      UINT64 ui8_segment_data_offset;
 *      UINT32 ui4_idx_type;
 *      INT32  i4_width;
 *      INT32  i4_height;
 *      UINT32 ui4_strm_id;
 *      UINT32 ui4_scale;
 *      UINT32 ui4_rate;
 *      UINT32 ui4_codec_info_size;
 *      UINT8* pui1_codec_info;
 *      MINFO_INFO_VIDEO_ENC_T e_enc;
 *  }MINFO_TYPE_SCALE_INFO_T;
 *  @endcode
 *  @li@c  ui8_pb_duration                                   -
 *  @li@c  ui8_media_sz                                      -
 *  @li@c  ui8_timecode_scale                                -
 *  @li@c  ui8_idx_offset                                    -
 *  @li@c  ui8_idx_sz                                        -
 *  @li@c  ui8_data_offset                                   -
 *  @li@c  ui8_data_sz                                       -
 *  @li@c  ui8_segment_data_offset                           -
 *  @li@c  ui4_idx_type                                      -
 *  @li@c  i4_width                                          -
 *  @li@c  i4_height                                         -
 *  @li@c  ui4_strm_id                                       -
 *  @li@c  ui4_scale                                         -
 *  @li@c  ui4_rate                                          -
 *  @li@c  ui4_codec_info_size                               -
 *  @li@c  pui1_codec_info                                   -
 *  @li@c  e_enc                                             -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_TYPE_SCALE_INFO_T
{
    UINT64 ui8_pb_duration;/*clock 90000HZ*/
    UINT64 ui8_media_sz; /*file total sz*/
    UINT64 ui8_timecode_scale; /*mkv file segment timecode scale*/
    UINT64 ui8_idx_offset;
    UINT64 ui8_idx_sz;
    UINT64 ui8_data_offset;
    UINT64 ui8_data_sz;
    UINT64 ui8_segment_data_offset;
    UINT32 ui4_idx_type;
    INT32  i4_width;
    INT32  i4_height;
    UINT32 ui4_strm_id;
    UINT32 ui4_scale;
    UINT32 ui4_rate;
    UINT32 ui4_codec_info_size;
    UINT8* pui1_codec_info;
    MINFO_INFO_VIDEO_ENC_T e_enc;
}MINFO_TYPE_SCALE_INFO_T;
#endif

#if 1/*MKV DivXPlus*/
typedef struct _MINFO_TITLE_INFO_T
{
    UINT32                  ui4_title_idx;
    UINT64                  ui8_title_uid_high;
    UINT64                  ui8_title_uid_low;
    UINT32                  ui4_playlist_num;
    BOOL                    fg_hidden;
    BOOL                    fg_default;
    BOOL                    fg_enable;
    UINT32                  ui4_title_len;
    UTF16_T                 pwc_title_name[TITLE_NAME_MAX];
}MINFO_TITLE_INFO_T;

typedef struct _MINFO_PLAYLIST_INFO_T
{
    UINT32                  ui4_title_idx;
    UINT32                  ui4_playlist_idx;
    UINT64                  ui8_playlist_uid;
    UINT64                  ui8_total_time;
    UINT32                  ui4_chapter_num;
    BOOL                    fg_hidden;
    BOOL                    fg_default;
    BOOL                    fg_enable;
}MINFO_PLAYLIST_INFO_T;

typedef struct _MINFO_CHAP_INFO_T
{
    UINT32                  ui4_title_idx;
    UINT32                  ui4_playlist_idx;
    UINT32                  ui4_chap_idx;
    UINT64                  ui8_chap_uid;
    UINT32                  ui4_disp_num;
    UINT64                  ui8_start_time;
    UINT64                  ui8_end_time;
    UINT64                  ui8_pl_time;
    BOOL                    fg_hidden;
    BOOL                    fg_default;
    BOOL                    fg_enable;
}MINFO_CHAP_INFO_T;

typedef struct _MINFO_DISP_INFO_T
{
    UINT32                  ui4_title_idx;
    UINT32                  ui4_playlist_idx;
    UINT32                  ui4_chap_idx;
    UINT32                  ui4_disp_idx;
    UINT32                  ui4_chapter_len;
    CHAR                    ps_chapter_name[STRING_LEN_MAX];
    UINT32                  ui4_country_len;
    CHAR                    ps_country[STRING_LEN_MAX];
    UINT32                  ui4_language_len;
    CHAR                    ps_language[STRING_LEN_MAX];
}MINFO_DISP_INFO_T;
#endif

/*------------------------------------------------------------------*/
/*! @struct MINFO_TYPE_GENERIC_T
 *  @brief Media generic information.
 *  @code
 *  typedef struct _MINFO_TYPE_GENERIC_T
 *  {
 *      BOOL   b_is_drm_exist; 
 *      BOOL   b_hd_content;
 *      UINT64 ui8_pb_duration;
 *      UINT64 ui8_media_sz; 
 *      UINT8  ui1_strm_ns;  
 *      UINT64 ui8_preroll;
 *      UINT64 ui8_timecode_scale; 
 *      UINT32 ui4_sz_max_acces_unt; 
 *      UINT32 ui4_sz_min_acces_unt; 
 *  }MINFO_TYPE_GENERIC_T;
 *  @endcode
 *  @li@c  b_is_drm_exist                                    -
 *  @li@c  b_hd_content                                      -
 *  @li@c  ui8_pb_duration                                   -
 *  @li@c  ui8_media_sz                                      -
 *  @li@c  ui1_strm_ns                                       -
 *  @li@c  ui8_preroll                                       -
 *  @li@c  ui8_timecode_scale                                -
 *  @li@c  ui4_sz_max_acces_unt                              -
 *  @li@c  ui4_sz_min_acces_unt                              -
 */
/*------------------------------------------------------------------*/

typedef struct _MINFO_TYPE_GENERIC_T
{
    BOOL   b_is_drm_exist; /*whether drm is exist, default is false*/
    BOOL   b_hd_content;
    BOOL   b_non_seekable; /* tell swdmx whether does server support seek*/
    BOOL   b_is_bad_interleaved;  /*Bad interleaved Flag*/
    UINT64 ui8_pb_duration;/*clock 90000HZ*/
    UINT64 ui8_media_sz; /*file total sz*/
    UINT8  ui1_strm_ns;  /*total stream numbers in media*/
    UINT64 ui8_preroll;
    UINT64 ui8_timecode_scale; /*mkv file segment timecode scale*/
    UINT32 ui4_sz_max_acces_unt; /*for asf playback*/
    UINT32 ui4_sz_min_acces_unt; /*for asf playback*/
    UINT32 ui4_sz_max_bitrate;   /*for asf playback*/
    /*add for 3d media*/
    BOOL b_stereo_scopic;
    BOOL b_half_width;
    BOOL b_half_height;
    MM_SCOPIC_TYPE_T e_scopic_type;
    UINT64  ui8_skip_dur;  
}MINFO_TYPE_GENERIC_T;
#if 0
/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MINFO_TYPE_TIME_T
 *  {
 *      UINT16 ui2_yr;
 *      UINT8  ui1_mo;      
 *      UINT8  ui1_day;     
 *      UINT8  ui1_dow;     
 *      UINT8  ui1_hr;      
 *      UINT8  ui1_min;     
 *      UINT8  ui1_sec;     
 *      BOOL   b_gmt;       
 *      BOOL   b_dst;       
 *  } MINFO_TYPE_TIME_T;
 *  @endcode
 *  @li@c  ui2_yr                                            -
 *  @li@c  ui1_mo                                            -
 *  @li@c  ui1_day                                           -
 *  @li@c  ui1_dow                                           -
 *  @li@c  ui1_hr                                            -
 *  @li@c  ui1_min                                           -
 *  @li@c  ui1_sec                                           -
 *  @li@c  b_gmt                                             -
 *  @li@c  b_dst                                             -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_TYPE_TIME_T
{
    UINT16 ui2_yr;
    UINT8  ui1_mo;      /* 1-12, month of the year. */
    UINT8  ui1_day;     /* 1-31, day of the month. */
    UINT8  ui1_dow;     /* 0-6, Sunday to Saturday. */
    UINT8  ui1_hr;      /* 0-23 */
    UINT8  ui1_min;     /* 0-59 */
    UINT8  ui1_sec;     /* 0-61 */

    BOOL   b_gmt;       /* TRUE:  Universal Time Coordinate (UTC)
                           FALSE: Local Time */
    BOOL   b_dst;       /* TRUE:  DayLight-Saving-Time on
                           FALSE: Day-Light_Saving Time off */
} MINFO_TYPE_TIME_T;
#endif

/*------------------------------------------------------------------*/
/*! @struct MM_TM_T
 *  @brief Media time information.
 *  @code
 *  typedef struct _MM_TM_T
 *  {
 *      UINT16 ui2_year;       
 *      UINT8  ui1_month;      
 *      UINT8  ui1_day;        
 *      UINT8  ui1_week;       
 *      UINT8  ui1_hour;       
 *      UINT8  ui1_min;        
 *      UINT8  ui1_sec;        
 *      BOOL   b_gmt;          
 *                                       Greenwich Mean Time(GMT)
 *                                FALSE: Local Time
 *      BOOL   b_dst;          
 *                                FALSE: Day-Light_Saving Time off
 *  }MM_TM_T ;
 *  @endcode
 *  @li@c  ui2_year                                          -
 *  @li@c  ui1_month                                         -
 *  @li@c  ui1_day                                           -
 *  @li@c  ui1_week                                          -
 *  @li@c  ui1_hour                                          -
 *  @li@c  ui1_min                                           -
 *  @li@c  ui1_sec                                           -
 *  @li@c  b_gmt                                             -
 *  @li@c  b_dst                                             -
 */
/*------------------------------------------------------------------*/
typedef struct _MM_TM_T
{
    UINT16 ui2_year;       /* This is a absolute year value e.g. 2008*/
    UINT8  ui1_month;      /* 1-12, month of the year. */
    UINT8  ui1_day;        /* 1-31, day of the month.  */
    UINT8  ui1_week;       /* 0-6, Sunday to Saturday. */
    UINT8  ui1_hour;       /* 0-23 */
    UINT8  ui1_min;        /* 0-59 */
    UINT8  ui1_sec;        /* 0-59, permit 61 */

    BOOL   b_gmt;          /* TRUE:  Universal Time Coordinate(UTC)
                                     Greenwich Mean Time(GMT)
                              FALSE: Local Time
                           */
    BOOL   b_dst;          /* TRUE:  DayLight-Saving-Time on
                              FALSE: Day-Light_Saving Time off
                           */
}MM_TM_T ;


/*------------------------------------------------------------------*/
/*! @struct MINFO_TYPE_FRAME_RATE_T
 *  @brief Frame rate.
 *  @code
 *  typedef struct _MINFO_TYPE_FRAME_RATE_T
 *  {
 *      UINT32  ui4_rate;
 *      UINT32  ui4_scale;
 *  }MINFO_TYPE_FRAME_RATE_T;
 *  @endcode
 *  @li@c  ui4_rate                                          -
 *  @li@c  ui4_scale                                         -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_TYPE_FRAME_RATE_T
{
    UINT32  ui4_rate;
    UINT32  ui4_scale;
}MINFO_TYPE_FRAME_RATE_T;

/*------------------------------------------------------------------*/
/*! @struct MINFO_TYPE_VIDEO_ES_INFO_T
 *  @brief Video es information.
 *  @code
 *  typedef struct _MINFO_TYPE_VIDEO_ES_INFO_T
 *  {
 *      INT32 i4_video_inf_width;
 *      INT32 i4_video_inf_height;
 *  }MINFO_TYPE_VIDEO_ES_INFO_T;
 *  @endcode
 *  @li@c  i4_video_inf_width                                -
 *  @li@c  i4_video_inf_height                               -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_TYPE_VIDEO_ES_INFO_T
{
    INT32 i4_video_inf_width;
    INT32 i4_video_inf_height;
}MINFO_TYPE_VIDEO_ES_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct MINFO_TYPE_AUDIO_ES_INFO_T
 *  @brief Audio es information.
 *  @code
 *  typedef struct _MINFO_TYPE_AUDIO_ES_INFO_T
 *  {
 *  }MINFO_TYPE_AUDIO_ES_INFO_T;
 *  @endcode
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_TYPE_AUDIO_ES_INFO_T
{/*no member currently, maybe use later*/
    UINT8 ui1_test;/*for build only*/
}MINFO_TYPE_AUDIO_ES_INFO_T;



/*------------------------------------------------------------------*/
/*! @struct MINFO_TYPE_META_DATA_T
 *  @brief Media meta data information.
 *  @code
 *  typedef struct _MINFO_TYPE_META_DATA_T
 *  {
 *      SIZE_T                  z_title_len;
 *      UTF16_T*                pwc_str_title;
 *      SIZE_T                  z_director_len;
 *      UTF16_T*                pwc_str_director;
 *      SIZE_T                  z_copyright_len;
 *      UTF16_T*                pwc_str_copyright;
 *      SIZE_T                  z_genre_len;
 *      UTF16_T*                pwc_str_genre;
 *      BOOL                    b_cr_date_exist;
 *      MM_TM_T                 t_date;
 *      MINFO_TYPE_FRAME_RATE_T t_frame_rate;
 *      UINT64                  ui8_pb_duration;
 *      UINT32                  ui4_bitrate;
 *      MINFO_INFO_PIC_T        t_id3_attatched_pic;
 *      SIZE_T                  z_artist_len;
 *      UTF16_T*                pwc_str_artist;
 *      SIZE_T                  z_album_len;
 *      UTF16_T*                pwc_str_album;
 *      SIZE_T                  z_track_len;
 *      UTF16_T*                pwc_str_track;
 *      SIZE_T                  z_year_len;
 *      UTF16_T*                pwc_str_year;
 *      MINFO_TYPE_VIDEO_ES_INFO_T   t_video_es_info;
 *      MINFO_TYPE_AUDIO_ES_INFO_T   t_audio_es_info;
 *  }MINFO_TYPE_META_DATA_T;
 *  @endcode
 *  @li@c  z_title_len                                       -
 *  @li@c  pwc_str_title                                     -
 *  @li@c  z_director_len                                    -
 *  @li@c  pwc_str_director                                  -
 *  @li@c  z_copyright_len                                   -
 *  @li@c  pwc_str_copyright                                 -
 *  @li@c  z_genre_len                                       -
 *  @li@c  pwc_str_genre                                     -
 *  @li@c  b_cr_date_exist                                   -
 *  @li@c  t_date                                            -
 *  @li@c  t_frame_rate                                      -
 *  @li@c  ui8_pb_duration                                   -
 *  @li@c  ui4_bitrate                                       -
 *  @li@c  t_id3_attatched_pic                               -
 *  @li@c  z_artist_len                                      -
 *  @li@c  pwc_str_artist                                    -
 *  @li@c  z_album_len                                       -
 *  @li@c  pwc_str_album                                     -
 *  @li@c  z_track_len                                       -
 *  @li@c  pwc_str_track                                     -
 *  @li@c  z_year_len                                        -
 *  @li@c  pwc_str_year                                      -
 *  @li@c  t_video_es_info                                   -
 *  @li@c  t_audio_es_info                                   -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_TYPE_META_DATA_T
{
    SIZE_T                  z_title_len;
    UTF16_T*                pwc_str_title;
    SIZE_T                  z_director_len;
    UTF16_T*                pwc_str_director;
    SIZE_T                  z_copyright_len;
    UTF16_T*                pwc_str_copyright;
    SIZE_T                  z_genre_len;
    UTF16_T*                pwc_str_genre;
    SIZE_T                  z_rating_len;    
    UTF16_T*                pwc_str_rating;
    BOOL                    b_cr_date_exist;
    MM_TM_T                 t_date;
    MINFO_TYPE_FRAME_RATE_T t_frame_rate;
    UINT64                  ui8_pb_duration;
    UINT32                  ui4_bitrate;

    MINFO_INFO_PIC_T        t_id3_attatched_pic;
    SIZE_T                  z_artist_len;
    UTF16_T*                pwc_str_artist;
    SIZE_T                  z_album_len;
    UTF16_T*                pwc_str_album;
    SIZE_T                  z_track_len;
    UTF16_T*                pwc_str_track;
    SIZE_T                  z_year_len;
    UTF16_T*                pwc_str_year;

    MINFO_TYPE_VIDEO_ES_INFO_T   t_video_es_info;
    MINFO_TYPE_AUDIO_ES_INFO_T   t_audio_es_info;
}MINFO_TYPE_META_DATA_T;


#define MAX_SUPPORT_FILE_NUM    64
/* used to store multi file information*/
typedef struct _MP4_SINGLE_FILE_INFO_T
{
    UINT64  ui8_file_start_pos;      /* Each file start offset */
    UINT64  ui8_file_start_pts;      /* Each file start pts    */
    UINT64  ui8_file_duration;       /* Each file start pts    */
    UINT64  ui8_file_moov_pos;       /* Each file moov offset  */
    UINT64  ui8_file_moov_size;      /* Each file moov size    */
} MP4_SINGLE_FILE_INFO_T;


/* used to store multi file information*/
typedef struct _MP4_MULTI_FILE_INFO_T
{
    UINT8   ui1_file_num;
    MP4_SINGLE_FILE_INFO_T  at_file_info[MAX_SUPPORT_FILE_NUM];
} MP4_MULTI_FILE_INFO_T;

typedef struct 
{
    VOID*                   pt_mp4_cmov_info;       /*for mp4 cmov only*/
    VOID*                   pt_piff_inet_info;      /*for piff inet only*/
    VOID*                   pt_mp4_multi_file_info; /*for mp4 seamless playback only*/    
} MP4_MINFO_EXTRA_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct MINFO_TYPE_IDX_INFO_T
 *  @brief Media indexing information.
 *  @code
 *  typedef struct _MINFO_TYPE_IDX_INFO_T
 *  {
 *      UINT32 ui4_idx_type;
 *      UINT64 ui8_idx_offset;
 *      UINT64 ui8_idx_sz;
 *      UINT64 ui8_data_offset;
 *      UINT64 ui8_data_sz;
 *      UINT64 ui8_segment_data_offset; 
 *      UINT64 ui8_seg_timecode_scale;  
 *      VOID*  pt_mp4_cmov_info; 
 *  }MINFO_TYPE_IDX_INFO_T;
 *  @endcode
 *  @li@c  ui4_idx_type                                      -
 *  @li@c  ui8_idx_offset                                    -
 *  @li@c  ui8_idx_sz                                        -
 *  @li@c  ui8_data_offset                                   -
 *  @li@c  ui8_data_sz                                       -
 *  @li@c  ui8_segment_data_offset                           -
 *  @li@c  ui8_seg_timecode_scale                            -
 *  @li@c  pt_mp4_cmov_info                                  -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_TYPE_IDX_INFO_T
{
    UINT32 ui4_idx_type;
    UINT64 ui8_idx_offset;
    UINT64 ui8_idx_sz;
    UINT64 ui8_data_offset;
    UINT64 ui8_data_sz;
#ifdef RM_LOGICAL_STREAM_SUPPORT
    UINT64 ui8_data_offset2;
    UINT64 ui8_data_sz2;
#endif
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    UINT32 ui4_1st_pkgnum;
#endif
    UINT64 ui8_segment_data_offset; /*the mkv file segment data start position*/
    UINT64 ui8_seg_timecode_scale;  /*the mkv file segment timecode scale*/
    MP4_MINFO_EXTRA_INFO_T  t_mp4_extra_info;
    /*For asf  index object use, if the most significant bit is set then index obejcts exsits,
          the other bits indicates non-hidden stream number related to simple index objects
          aui1_idx_strm_map[0].bit0 is most.*/
    UINT8 aui1_idx_strm_map[16]; 
}MINFO_TYPE_IDX_INFO_T;

typedef struct
{
    UINT8*  pui1_ct_id;              /* octopus content id           ---8id box content         */
    UINT32  ui4_ct_id_length;        /* length of octopus content id ---8id box content length-1*/
    UINT8   ui1_ipmp_id;             /* ipmp descriptor id           ---the byte before 0xA551  */
    UINT32  ui4_satr_algo_type;      /* algorithm type               ---HMAC size==32 ???       */
    UINT8*  pui1_satr_ctm;           /* content metadata,i.e satr_box---satr box content        */
    UINT32  ui4_satr_ctm_length;     /* length of content metadata   ---satr box size-8         */
    UINT8*  pui1_satr_hmac_val;      /* hmac value, i.e satr_hmac, length is 32 bytes */
}MINFO_MP4_TRAK_ENCRYTION_INFO_T;

#define     MINFO_MP4_VID_TRAK_ENCPT_INFO_T     MINFO_MP4_TRAK_ENCRYTION_INFO_T
#define     MINFO_MP4_AUD_TRAK_ENCPT_INFO_T     MINFO_MP4_TRAK_ENCRYTION_INFO_T

typedef struct _MINFO_MP4_ENCRYTION_INFO_T
{
    UINT32      ui4_strm_id;
    union
    {
        MINFO_MP4_VID_TRAK_ENCPT_INFO_T  t_video_encrytion_info;   
        MINFO_MP4_AUD_TRAK_ENCPT_INFO_T  t_audio_encrytion_info; 
    }u;
}MINFO_MP4_ENCRYTION_INFO_T;

typedef enum
{
    MINFO_STRM_TYPE_AUDIO,
    MINFO_STRM_TYPE_VIDEO,
    MINFO_STRM_TYPE_SUBTL
}MINFO_ENCRYPT_STRM_TYPE_T;

typedef struct _MINFO_TYPE_ENCRYTION_INFO_T
{
    UINT8                       ui1_strm_idx;
    MINFO_ENCRYPT_STRM_TYPE_T   e_strm_type;
    union MINFO_ENCRYTION_UNION_T
    {
        MINFO_MP4_ENCRYTION_INFO_T  t_minfo_mp4_encry_info;
    }u;
}MINFO_TYPE_ENCRYPTION_INFO_T;


/* This structure defines the divx drm object for minfo. */
/*------------------------------------------------------------------*/
/*! @struct MINFO_DIVX_DRM_OBJ_T
 *  @brief Drm strd information.
 *  @code
 *  typedef struct _MINFO_DIVX_DRM_OBJ_T
 *  {
 *      UINT8* pui1_strd_chunk;
 *  }MINFO_DIVX_DRM_OBJ_T;
 *  @endcode
 *  @li@c  pui1_strd_chunk                                   -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_DIVX_DRM_OBJ_T
{
    UINT8* pui1_strd_chunk;
}MINFO_DIVX_DRM_OBJ_T;

/* This structure defines the drm info for minfo. */
/*------------------------------------------------------------------*/
/*! @struct MINFO_TYPE_DRM_INFO_T
 *  @brief Drm information.
 *  @code
 *  typedef struct _MINFO_TYPE_DRM_INFO_T
 *  {
 *      DRM_TYPE_T t_minfo_drm_type;
 *      union MINFO_DRM_UNION_T
 *      {
 *          MINFO_DIVX_DRM_OBJ_T t_minfo_divx_drm_obj;
 *      }u;
 *  }MINFO_TYPE_DRM_INFO_T;
 *  @endcode
 *  @li@c  t_minfo_drm_type                                  -
 *  @li@c  t_minfo_divx_drm_obj                              -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_TYPE_DRM_INFO_T
{
    DRM_TYPE_T t_minfo_drm_type;
    union MINFO_DRM_UNION_T
    {
        MINFO_DIVX_DRM_OBJ_T t_minfo_divx_drm_obj;
    }u;
}MINFO_TYPE_DRM_INFO_T;

/*add for font info*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_TYPE_FONT_INFO_T
 *  @brief Font information.
 *  @code
 *  typedef struct _MINFO_TYPE_FONT_INFO_T
 *  {
 *      FONT_INFO_T* pt_font_info;
 *  }MINFO_TYPE_FONT_INFO_T;
 *  @endcode
 *  @li@c  pt_font_info                                      -
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_TYPE_FONT_INFO_T
{
    FONT_INFO_T* pt_font_info;
}MINFO_TYPE_FONT_INFO_T;

/* add for divx type*/
typedef struct _MINFO_TYPE_DIVX_INFO_T
{
    MINFO_INFO_CODEC_ENC_T  t_minfo_info_codec_enc;
}MINFO_TYPE_DIVX_INFO_T;

/*This structure define media infomation which  media info utility support*/
/*------------------------------------------------------------------*/
/*! @struct MINFO_INFO_T
 *  @brief Media information.
 *  @code
 *  typedef struct _MINFO_INFO_T
 *  {
 *      MEDIA_FORMAT_T              t_format;        
 *      MINFO_INFO_TYPE_T           e_info_type;     
 *      UINT8                       ui1_rec_idx;     
 *      union MINFO_REC_UNION_T
 *      {
 *          MINFO_TYPE_GENERIC_T    t_general_info;  
 *          MINFO_TYPE_META_DATA_T  t_meta_data;     
 *          MINFO_TYPE_STM_ATRBT_T  t_stm_attribute; 
 *          MINFO_TYPE_ID3_TAG_T    t_id3_data;      
 *          MINFO_TYPE_IDX_INFO_T   t_idx_info;      
 *          MINFO_TYPE_DRM_INFO_T   t_drm_info;      
 *          MINFO_TYPE_FONT_INFO_T  t_font_info;     
 *  #if SUPPORT_MULTI_INFO
 *          MINFO_TYPE_SCALE_INFO_T t_scale_info;  
 *  #endif
 *      }u;
 *  }MINFO_INFO_T;
 *  @endcode
 *  @li@c  t_format                                          - Media format.
 *  @li@c  e_info_type                                       - Infor type.
 *  @li@c  ui1_rec_idx                                       - Record index.
 *  @li@c  t_general_info                                    - General infor.
 *  @li@c  t_meta_data                                       - Meta data.
 *  @li@c  t_stm_attribute                                   - Stream attribute.
 *  @li@c  t_id3_data                                        - Id3 data.
 *  @li@c  t_idx_info                                        - Indexing infor.
 *  @li@c  t_drm_info                                        - Drm infor.
 *  @li@c  t_font_info                                       - Font infor.
 *  @li@c  t_scale_info                                      - Scale infor.
 */
/*------------------------------------------------------------------*/
typedef struct _MINFO_INFO_T
{
    MEDIA_FORMAT_T              t_format;        /*O*/
    MINFO_INFO_TYPE_T           e_info_type;     /*O*/
    UINT8                       ui1_rec_idx;     /*O*/
    union MINFO_REC_UNION_T
    {
        MINFO_TYPE_GENERIC_T    t_general_info;  /*O*/
        MINFO_TYPE_META_DATA_T  t_meta_data;     /*0*/
        MINFO_TYPE_STM_ATRBT_T  t_stm_attribute; /*O*/
        MINFO_TYPE_ID3_TAG_T    t_id3_data;      /*O*/
        MINFO_TYPE_IDX_INFO_T   t_idx_info;      /*O*/
        MINFO_TYPE_DRM_INFO_T   t_drm_info;      /*O*/
        MINFO_TYPE_ENCRYPTION_INFO_T   t_encrytion_info;      /*O*/    
        /*add for font info*/
        MINFO_TYPE_FONT_INFO_T  t_font_info;     /*O*/
#if SUPPORT_MULTI_INFO
        MINFO_TYPE_SCALE_INFO_T t_scale_info;  /*O*/
#endif
        MINFO_TYPE_DIVX_INFO_T  t_divx_info;
    }u;
}MINFO_INFO_T;

typedef struct _MINFO_TYPE_RAW_DATA_T
{
    UINT64              ui8_seek_pos;
    VOID*               pv_get_info;
    SIZE_T              z_read_data_size;
}MINFO_TYPE_RAW_DATA_T;

/************************************************************************/
/* Error code                                                           */
/************************************************************************/
#define MINFOR_INFO_NOT_FOUND            ((INT32)    3)    /**<        */
#define MINFOR_SKIP_FRAME                ((INT32)    2)    /**<        */
#define MINFOR_EOT                       ((INT32)    1)    /**<        */
#define MINFOR_OK                        ((INT32)    0)    /**<        */
#define MINFOR_INSUFFICIENT_BUFFER       ((INT32)   -1)    /**<        */
#define MINFOR_FILE_OR_DIR_MISSING       ((INT32)   -2)    /**<        */
#define MINFOR_INV_HANDLE                ((INT32)   -3)    /**<        */
#define MINFOR_OUT_OF_HANDLE             ((INT32)   -4)    /**<        */
#define MINFOR_OUT_OF_MEMORY             ((INT32)   -5)    /**<        */
#define MINFOR_FCT_NOT_SUPPORT           ((INT32)   -6)    /**<        */
#define MINFOR_NAME_EXIST                ((INT32)   -7)    /**<        */
#define MINFOR_INV_ARG                   ((INT32)   -8)    /**<        */
#define MINFOR_HANDLER_NOT_FOUND         ((INT32)   -9)    /**<        */
#define MINFOR_INTERNAL_ERR              ((INT32)   -10)    /**<        */
#define MINFOR_REENTRY_LOADING           ((INT32)   -11)    /**<        */
#define MINFOR_INITIAL_ERR               ((INT32)   -12)    /**<        */
#define MINFOR_MSG_Q_ERR                 ((INT32)   -13)    /**<        */
#define MINFOR_FILE_ERR                  ((INT32)   -14)    /**<        */
#define MINFOR_CLI_ERROR                 ((INT32)   -15)    /**<        */
#define MINFOR_AUTOFREE_ERROR            ((INT32)   -16)    /**<        */
#define MINFOR_FMT_NOT_SUPPORT           ((INT32)   -17)    /**<        */
#define MINFOR_INFO_NOT_READY            ((INT32)   -18)    /**<        */
#define MINFOR_OUT_OF_RECORD             ((INT32)   -19)    /**<        */
#define MINFOR_MOOV_SIZE_EXCE_MAX        ((INT32)   -20)    /**<        */

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/* This callback function is the media info module notification function. This will be called
when minfo status changed. Parsing in minfo is asynchronous functions, so it needs this
notification function to notify applications. After Parsing done, AP can get the information or
it will return MINFOR_NOT_READY */
/*------------------------------------------------------------------*/
/*! @brief Minfo notify function when infor parse finished.
 *  @param [in] h_minfo  - Specifies which handle will be notified.
 *  @param [in] e_event  - It identifies the event type of notification.
 *  @param [in] pv_data1  - Specify the notification data1 which is followed with the notification event. 
 *  @param [in] pv_data2  - Specify the notification data1 which is followed with the notification event. 
 *  @param [in] pv_tag  - The client data specified by the client when callback has been called, the callback function will send it again.
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_minfo_nfy_fct) (
                        HANDLE_T                    h_minfo,  /* (I) Specifies which handle will be notified. */
                        MINFO_NFY_EVENT_TYPE_T      e_event,  /* (I) it identifies the event type of notification. */
                        const VOID*                 pv_data1, /* (I) specify the notification data1 which is followed with the notification event. */
                        const VOID*                 pv_data2, /* (I) specify the notification data2 which is followed with the notification event. */
                        const VOID*                 pv_tag);  /* (I) The client data specified by the client when callback has been called, the callback function will send it again. */

/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_MUTIL_MINFO */
/*----------------------------------------------------------------------------*/

/*! @page pageMW_MUTIL_MINFO Media Information
 *  @section secMW_MUTIL_MINFO_1  1. Introduction
 *  @par
 *      Minfo code to get media information, if current file format is unknown, it will get file format first.
 *  @par
 *  @section secMW_MUTIL_MINFO_2  2. Concept
 *  @par
 *      @include minfo_sample_code.c
 *      
 */
#endif /* _U_MINFO_H_ */

