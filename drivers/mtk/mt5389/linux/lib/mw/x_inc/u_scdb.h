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
/*! @file u_scdb.h 
 *  $File: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/middleware/scdb/u_scdb.h $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains stream component database API definitions,
 *         which are exported to other Middleware components and applications.
 *
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SCDB_H_
#define _U_SCDB_H_
/*----------------------------------------------------------------------------*/
/*! @defgroup   groupMW_SCDB Stream Component Database
 *  @ingroup    groupMW
 *  @brief      In the middleware layer, SCDB will be created (by the Connection
 *              Manager) to store information about stream components (e.g.
 *              video streams, audio streams, subtitle streams) for a given
 *              connection.  Applications, Stream Manager, and Process Manager
 *              can access SCDB to obtain information about the available stream
 *              components for specific connection.
 *  @see        groupMW_CDB, groupMW_CONN_MANAGER
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"
#include "u_cdb.h"
#include "u_minfo.h"

/*--------------------------------------------------------------------------
 Constants, enumerations, and macros
---------------------------------------------------------------------------*/

/* Max len for language identifier string: ISO_639_LANG_LEN (4 bytes).*/
#define   MAX_LANG_LEN   3    /**< Max len for language identifier string: ISO_639_LANG_LEN (4 bytes). */

/* Return code status from API */
#define         SCDBR_OK                   ((INT32)  0)     /**< Operation succeeded. */
#define	        SCDBR_FAIL			       ((INT32) -1)     /**< Operation failed. */
#define         SCDBR_OUT_OF_HANDLE        ((INT32) -2)     /**< Handle allocation failed. */
#define	        SCDBR_OUT_OF_MEM           ((INT32) -3)     /**< Memory exhausted. */
#define	        SCDBR_INV_HANDLE           ((INT32) -4)     /**< Bad handle. */
#define         SCDBR_INV_ARG              ((INT32) -5)     /**< Invalid argument. */
#define	        SCDBR_REC_NOT_FOUND        ((INT32) -6)     /**< Record not found. */
#define	        SCDBR_NO_WRITE_LOCK        ((INT32) -7)     /**< Failed initializing RW Lock. */
#define         SCDBR_NO_PERMISSION        ((INT32) -8)     /**< No permission as only the creator can modify SCDB. */
#define         SCDBR_DB_MODIFIED          ((INT32) -9)     /**< Database modified since last iteration. */
#define         SCDBR_DB_LIMIT_EXCEED      ((INT32) -10)    /**< Reached maximum number of databases. */
#define         SCDBR_NOT_FOUND            ((INT32) -11)    /**< Record not found. */
#define         SCDBR_AEE_OUT_OF_RESOURCES ((INT32) -12)    /**< Can not allocate AEE resources. */
#define         SCDBR_AEE_NO_RIGHTS        ((INT32) -13)    /**< No right for the AEE resources. */
#define         SCDBR_BUF_TOO_SMALL        ((INT32) -14)    /**< Buffer too small to hold all the values. */

/* Handle type for the SCDB handle. */
#define SCDBT_STREAM_DB  (HT_GROUP_SCDB + ((HANDLE_TYPE_T) 0))    /**< Handle type for the SCDB. */

/* SCDB condition */
/*------------------------------------------------------------------*/
/*! @brief  Enumeration for SCDB conditions.
 *  @code
 *  typedef  enum
 *  {
 *      SCDB_UPDATING = 1,  
 *      SCDB_UPDATED,       
 *      SCDB_CLOSED,        
 *      SCDB_UNKNOWN
 *  } SCDB_COND_T;
 *  @endcode
 *  @li@c  SCDB_UPDATING                 - When scdb_lock is called.
 *  @li@c  SCDB_UPDATED                  - When scdb_unlock is called.
 *  @li@c  SCDB_CLOSED                   - When scdb_delete is called.
 *  @li@c  SCDB_UNKNOWN                  - Unknown condition.
 */
/*------------------------------------------------------------------*/
typedef  enum
{
    SCDB_UPDATING = 1,  /* When scdb_lock is called. */
    SCDB_UPDATED,       /* When scdb_unlock is called. */
    SCDB_CLOSED,        /* When scdb_delete is called. */
    SCDB_UNKNOWN
} SCDB_COND_T;

/* Notification reasons */
#define   SCDB_REASON_UNK       ((UINT32)(0x0000))       /**< Reason unknown */
#define   SCDB_RECORD_ADD       (MAKE_BIT_MASK_32(1))    /**< Reason for adding records */
#define   SCDB_RECORD_DEL       (MAKE_BIT_MASK_32(2))    /**< Reason for deleting records */
#define   SCDB_RECORD_MOD       (MAKE_BIT_MASK_32(3))    /**< Reason for modifying records */
#define   SCDB_COMMON_MOD       (MAKE_BIT_MASK_32(4))    /**< Reason for modifying common data */


/*
   SCDB record type. This type contains tag value
   for the content of SCDB record union field.
*/
typedef  UINT8  SCDB_REC_TYPE_T;    /**< SCDB record type. This type contains tag value for the content of SCDB record union field. */

#define         SCDB_REC_TYPE_UNKNOWN            ((SCDB_REC_TYPE_T) 0)    /**< Unknown type */
#define         SCDB_REC_TYPE_AUDIO_MPEG         ((SCDB_REC_TYPE_T) 1)    /**< Type for MPEG audio */
#define         SCDB_REC_TYPE_VIDEO_MPEG         ((SCDB_REC_TYPE_T) 2)    /**< Type for MPEG video */
#define         SCDB_REC_TYPE_CLOSED_CAPTION     ((SCDB_REC_TYPE_T) 3)    /**< Type for closed caption */
#define         SCDB_REC_TYPE_AUDIO_ANALOG       ((SCDB_REC_TYPE_T) 4)    /**< Type for analog audio */
#define         SCDB_REC_TYPE_VIDEO_ANALOG       ((SCDB_REC_TYPE_T) 5)    /**< Type for analog video */
#define         SCDB_REC_TYPE_SUBTITLE_DVB       ((SCDB_REC_TYPE_T) 6)    /**< Type for DVB subtitle */
#define         SCDB_REC_TYPE_TELETEXT_DVB       ((SCDB_REC_TYPE_T) 7)    /**< Type for DVB teletext */
#define         SCDB_REC_TYPE_VBI_TELETEXT_DVB   ((SCDB_REC_TYPE_T) 8)    /**< Type for DVB VBI teletext */
#define         SCDB_REC_TYPE_VBI_DATA_DVB       ((SCDB_REC_TYPE_T) 9)    /**< Type for DVB VBI data */
#define         SCDB_REC_TYPE_PRIVATE_MPEG       ((SCDB_REC_TYPE_T) 10)   /**< Type for MPEG private stream */
#define         SCDB_REC_TYPE_DSMCC_MPEG         ((SCDB_REC_TYPE_T) 11)   /**< Type for MPEG DSMCC */
#define         SCDB_REC_TYPE_AUDIO_DIGITAL      ((SCDB_REC_TYPE_T) 12)   /**< Type for digital audio */
#define         SCDB_REC_TYPE_AUDIO_AVI          ((SCDB_REC_TYPE_T) 13)   /**< Type for AVI audio */
#define         SCDB_REC_TYPE_VIDEO_AVI          ((SCDB_REC_TYPE_T) 14)   /**< Type for AVI video */
#define         SCDB_REC_TYPE_SUBTITLE_DIVX      ((SCDB_REC_TYPE_T) 15)   /**< Type for DivX subtitle */
#define         SCDB_REC_TYPE_AUDIO_MP4          ((SCDB_REC_TYPE_T) 16)   /**< Type for MP4 audio */
#define         SCDB_REC_TYPE_VIDEO_MP4          ((SCDB_REC_TYPE_T) 17)   /**< Type for MP4 video */
#define         SCDB_REC_TYPE_AUDIO_ASF          ((SCDB_REC_TYPE_T) 18)   /**< Type for ASF audio */
#define         SCDB_REC_TYPE_VIDEO_ASF          ((SCDB_REC_TYPE_T) 19)   /**< Type for ASF video */
#define         SCDB_REC_TYPE_AUDIO_PS           ((SCDB_REC_TYPE_T) 20)   /**< Type for Packeted Stream audio */
#define         SCDB_REC_TYPE_VIDEO_PS           ((SCDB_REC_TYPE_T) 21)   /**< Type for Packeted Stream video */
#define         SCDB_REC_TYPE_AUDIO_MKV          ((SCDB_REC_TYPE_T) 22)   /**< Type for MKV audio */
#define         SCDB_REC_TYPE_VIDEO_MKV          ((SCDB_REC_TYPE_T) 23)   /**< Type for MKV video */
#define         SCDB_REC_TYPE_SUBTITLE_MKV       ((SCDB_REC_TYPE_T) 24)   /**< Type for MKV subtitle */
#define         SCDB_REC_TYPE_AUDIO_MP3          ((SCDB_REC_TYPE_T) 25)   /**< Type for MP3 audio */
#define         SCDB_REC_TYPE_ISDB_CAPTION       ((SCDB_REC_TYPE_T) 26)   /**< Type for ISDB closed caption */
#define         SCDB_REC_TYPE_ISDB_TEXT          ((SCDB_REC_TYPE_T) 27)   /**< Type for ISDB superimpose */
#define         SCDB_REC_TYPE_VIDEO_ES           ((SCDB_REC_TYPE_T) 28)   /**< Type for Element Stream video */
#define         SCDB_REC_TYPE_AUDIO_ES           ((SCDB_REC_TYPE_T) 29)   /**< Type for Element Stream audio */
#define         SCDB_REC_TYPE_AUDIO_RM           ((SCDB_REC_TYPE_T) 30)   /**< Type for RM audio */
#define         SCDB_REC_TYPE_VIDEO_RM           ((SCDB_REC_TYPE_T) 31)   /**< Type for RM video */
#define         SCDB_REC_TYPE_SUBTITLE_EXT       ((SCDB_REC_TYPE_T) 32)   /**< Type for RM subtitle */
/*add for flv*/
#define         SCDB_REC_TYPE_AUDIO_FLV          ((SCDB_REC_TYPE_T) 33)   /**< Type for FLV audio */
#define         SCDB_REC_TYPE_VIDEO_FLV          ((SCDB_REC_TYPE_T) 34)   /**< Type for FLV video */

/*add for ogg*/
#define 	SCDB_REC_TYPE_AUDIO_OGG		 ((SCDB_REC_TYPE_T)	35)   /**< Type for OGG audio*/
#define 	SCDB_REC_TYPE_VIDEO_OGG		 ((SCDB_REC_TYPE_T)	36)   /**< Type for OGG video*/
#define     SCDB_REC_TYPE_SUBTITLE_MP4   ((SCDB_REC_TYPE_T) 37)   /**< Type for MP4 sp */



/* Data feed mode */
typedef  UINT8  FEED_MODE_T;    /**< Data feed mode */

#define         FEED_MODE_UNKNOWN     ((FEED_MODE_T) 0)    /**< Unknown feed mode */
#define         FEED_MODE_PUSH        ((FEED_MODE_T) 1)    /**< Feed mode push */
#define         FEED_MODE_PULL        ((FEED_MODE_T) 2)    /**< Feed mode pull */

/* Close-Caption Type */
typedef  UINT8  CC_TYPE_T;    /**< Close-Caption Type */

#define         CC_ATSC_LINE_21       ((CC_TYPE_T)  0)    /**< Line-21 CC */
#define         CC_ATSC_DTV           ((CC_TYPE_T)  1)    /**< DTV CC */

/* Close-Caption ID */
#define         CC_CC1                ((UINT8)  1)    /**< CC1 */
#define         CC_CC2                ((UINT8)  2)    /**< CC2 */
#define         CC_CC3                ((UINT8)  3)    /**< CC3 */
#define         CC_CC4                ((UINT8)  4)    /**< CC4 */

/*
   DVB SubTitle Type and defines for aspect ratio and for
   aspect ratio with Hard-Of-Hearing.
*/
typedef  UINT8  DVB_SBTL_TYPE_T;    /**< DVB SubTitle Type and defines for aspect ratio and for aspect ratio with Hard-Of-Hearing. */

#define         DVB_SBTL_UNKNOWN           ((DVB_SBTL_TYPE_T) 0)    /**< Unknown subtitle type */
#define         DVB_SBTL_NO_ASP_RATIO      ((DVB_SBTL_TYPE_T) 1)    /**< DVB subtitles (normal) with no monitor aspect ratio critically */
#define         DVB_SBTL_4_3               ((DVB_SBTL_TYPE_T) 2)    /**< DVB subtitles (normal) for display on 4:3 aspect ratio monitor */
#define         DVB_SBTL_16_9              ((DVB_SBTL_TYPE_T) 3)    /**< DVB subtitles (normal) for display on 16:9 aspect ratio monitor */
#define         DVB_SBTL_221_1             ((DVB_SBTL_TYPE_T) 4)    /**< DVB subtitles (normal) for display on 2.21:1 aspect ratio monitor */
#define         DVB_SBTL_HD                ((DVB_SBTL_TYPE_T) 5)    /**< DVB subtitles (normal) for display on a high definition monitor */
/* Aspect ratio for Hard-Of-Hearing. */
#define         DVB_SBTL_NO_ASP_RATIO_HOH  ((DVB_SBTL_TYPE_T) 6)    /**< DVB subtitles (for the hard of hearing) with no monitor aspect ratio critically */
#define         DVB_SBTL_4_3_HOH           ((DVB_SBTL_TYPE_T) 7)    /**< DVB subtitles (for the hard of hearing) for display on 4:3 aspect ratio monitor */
#define         DVB_SBTL_16_9_HOH          ((DVB_SBTL_TYPE_T) 8)    /**< DVB subtitles (for the hard of hearing) for display on 16:9 aspect ratio monitor */
#define         DVB_SBTL_221_1_HOH         ((DVB_SBTL_TYPE_T) 9)    /**< DVB subtitles (for the hard of hearing) for display on 2.21:1 aspect ratio monitor */
#define         DVB_SBTL_HD_HOH            ((DVB_SBTL_TYPE_T) 10)   /**< DVB subtitles (for the hard of hearing) for display on a high definition monitor */


/* Defines for DVB's TELETEXT and DVB's VBI Teletext page types */
typedef  UINT8  DVB_TLTXT_TYPE_T ;    /**< DVB's TELETEXT and DVB's VBI Teletext page types. */

#define         DVB_TLTXT_UNKNOWN               ((DVB_TLTXT_TYPE_T)  0)    /**< Unknown teletext type */
#define         DVB_TLTXT_INITIAL_PAGE          ((DVB_TLTXT_TYPE_T)  1)    /**< Initial teletext page */
#define         DVB_TLTXT_SBTL_PAGE             ((DVB_TLTXT_TYPE_T)  2)    /**< Teletext subtitle page */
#define         DVB_TLTXT_ADDL_PAGE             ((DVB_TLTXT_TYPE_T)  3)    /**< Additional information page */
#define         DVB_TLTXT_PRG_SCHEDULE_PAGE     ((DVB_TLTXT_TYPE_T)  4)    /**< Programme schedule page */
#define         DVB_TLTXT_HEARING_IMPAIR_PAGE   ((DVB_TLTXT_TYPE_T)  5)    /**< Teletext subtitle page for the hearing impared */

typedef  UINT32 DVB_VBI_DATA_SERVICE_ID_T ;    /**< DVB's VBI data service types. */

#define         DVB_VBI_DATA_UNKNOWN            (MAKE_BIT_MASK_32(0))    /**< Unknown data service */
#define         DVB_VBI_DATA_TLTXT              (MAKE_BIT_MASK_32(1))    /**< EBU teletext */
#define         DVB_VBI_DATA_INVERT_TLTXT       (MAKE_BIT_MASK_32(2))    /**< Inverted teletext */
#define         DVB_VBI_DATA_VPS                (MAKE_BIT_MASK_32(3))    /**< Video Programme System */
#define         DVB_VBI_DATA_WSS                (MAKE_BIT_MASK_32(4))    /**< Wide Screen Signalling */
#define         DVB_VBI_DATA_CC                 (MAKE_BIT_MASK_32(5))    /**< Closed Caption */
#define         DVB_VBI_DATA_MONOCHROME         (MAKE_BIT_MASK_32(6))    /**< Monochrome 4:2:2 samples */


/* Enumeration Audio stream property */
typedef  UINT8  AUD_FMT_T;    /**< Audio stream property. */

#define         AUD_FMT_UNKNOWN       ((AUD_FMT_T)  0)    /**< Unknown audio format */
#define         AUD_FMT_MONO          ((AUD_FMT_T)  1)    /**< Monophonic audio */
#define         AUD_FMT_DUAL_MONO     ((AUD_FMT_T)  2)    /**< Dual mono audio */
#define         AUD_FMT_STEREO        ((AUD_FMT_T)  3)    /**< Stereo sound */
#define         AUD_FMT_TYPE_5_1      ((AUD_FMT_T)  4)    /**< 5.1 channel audio */
#define         AUD_FMT_SUBSTREAM     ((AUD_FMT_T)  5)    /**< E-AC3 substream audio */

/* Typedef and enumeration for Audio Encoding type. */
typedef  UINT8  AUD_ENC_T;    /**< Audio Encoding type. */

#define         AUD_ENC_UNKNOWN       ((AUD_ENC_T) 0)    /**< Unknown audio encoding */
#define         AUD_ENC_AC3           ((AUD_ENC_T) 1)    /**< Dolby AC-3 audio coding */
#define         AUD_ENC_MPEG_1        ((AUD_ENC_T) 2)    /**< MPEG-1 (ISO/IEC 11172-3) encoded audio */
#define         AUD_ENC_MPEG_2        ((AUD_ENC_T) 3)    /**< MPEG-2 (ISO/IEC 13818-3) encoded audio */
#define         AUD_ENC_PCM           ((AUD_ENC_T) 4)    /**< PCM audio */
#define         AUD_ENC_TV_SYS        ((AUD_ENC_T) 5)    /**< TV system specific audio type */
#define         AUD_ENC_DTS           ((AUD_ENC_T) 6)    /**< DTS audio coding */
#define         AUD_ENC_AAC           ((AUD_ENC_T) 7)    /**< AAC encoded audio */
#define         AUD_ENC_EU_CANAL_PLUS ((AUD_ENC_T) 8)    /**< Canal+ SCART-out audio */
#define         AUD_ENC_WMA_V1        ((AUD_ENC_T) 9)    /**< WMA V1 encoded audio */
#define         AUD_ENC_WMA_V2        ((AUD_ENC_T) 10)   /**< WMA V2 encoded audio */
#define         AUD_ENC_WMA_V3        ((AUD_ENC_T) 11)   /**< WMA V3 encoded audio */
#define         AUD_ENC_E_AC3         ((AUD_ENC_T) 12)   /**< Enhanced AC-3 encoded audio */
#define         AUD_ENC_LPCM          ((AUD_ENC_T) 13)   /**< Linear PCM audio */
#define         AUD_ENC_FM_RADIO      ((AUD_ENC_T) 14)   /**< FM radio audio */
#define         AUD_ENC_COOK          ((AUD_ENC_T) 15)   /**< Realmedia Cook encoded audio */
/*add for DTMB*/
#define         AUD_ENC_DRA           ((AUD_ENC_T) 16)   /**< DRA (Dynamic Resolution Adaption) encoded audio */

#define         AUD_ENC_VORBIS        ((AUD_ENC_T) 17)   /**<MKV encoded audio */
#define         AUD_ENC_WMA_PRO       ((AUD_ENC_T) 18)   /**< WMA PRO encoded audio */
#define         AUD_ENC_WMA_LOSSLESS  ((AUD_ENC_T) 19)   /**< WMA LOSSLESS encoded audio */
#define         AUD_ENC_AWB           ((AUD_ENC_T) 20)   /**< AMB encoded audio */
#define         AUD_ENC_AMR           ((AUD_ENC_T) 21)   /**< AMR encoded audio */


/* Typedef and enumeration for Video Encoding type. */
typedef  UINT8  VID_ENC_T;    /**< Video Encoding type. */

#define         VID_ENC_UNKNOWN       ((VID_ENC_T) 0)    /**< Unknown video encoding */
#define         VID_ENC_MPEG_1        ((VID_ENC_T) 1)    /**< MPEG-1 (ISO/IEC 11172-2) encoded video */
#define         VID_ENC_MPEG_2        ((VID_ENC_T) 2)    /**< MPEG-2 (ISO/IEC 13818-2) encoded video */
#define         VID_ENC_MPEG_4        ((VID_ENC_T) 3)    /**< MPEG-4 (ISO/IEC 14496-2) encoded video */
#define         VID_ENC_DIVX_311      ((VID_ENC_T) 4)    /**< DivX 3.11 encoding */
#define         VID_ENC_DIVX_4        ((VID_ENC_T) 5)    /**< DivX 4 encoding */
#define         VID_ENC_DIVX_5        ((VID_ENC_T) 6)    /**< DivX 5 encoding */
#define         VID_ENC_XVID          ((VID_ENC_T) 7)    /**< Xvid encoding */
#define         VID_ENC_WMV1          ((VID_ENC_T) 8)    /**< WMV1 (Windows Media Video v7) encoding */
#define         VID_ENC_WMV2          ((VID_ENC_T) 9)    /**< WMV2 (Windows Media Video v8) encoding */
#define         VID_ENC_WMV3          ((VID_ENC_T) 10)   /**< WMV3 (Windows Media Video v9) encoding */
#define         VID_ENC_WVC1          ((VID_ENC_T) 11)   /**< WVC1 (Windows Media Video v9 Advanced Profile) encoding */
#define         VID_ENC_H264          ((VID_ENC_T) 12)   /**< H.264 (ISO/IEC 14496-10) encoded video */
#define         VID_ENC_H263          ((VID_ENC_T) 13)   /**< H.263 encoded video */
#define         VID_ENC_MJPEG         ((VID_ENC_T) 14)   /**< Motion JPEG encoded video */
#define         VID_ENC_RV8           ((VID_ENC_T) 15)   /**< RealVideo 8 encoding */
#define         VID_ENC_RV9           ((VID_ENC_T) 16)   /**< RealVideo 9 encoding */
/*add for flv*/
#define         VID_ENC_SORENSON      ((VID_ENC_T) 17)   /**< Sorenson codec */
/*add for DTMB*/
#define         VID_ENC_AVS           ((VID_ENC_T) 18)   /**< AVS (Audio Video Standard) encoded video */
#define         VID_ENC_NV12          ((VID_ENC_T) 19)   /**< RAW encoding */
#define         VID_ENC_VP8           ((VID_ENC_T) 20)   /**< VP8 encoded video for MKV */
#define         VID_ENC_VP6           ((VID_ENC_T) 21)   /**< VP6 encoded video for FLV */


/* Typedef and enumeration for the Audio layer properties. */
typedef  UINT8  AUD_LAYER_T;    /**< Audio layer properties. */

#define         AUD_LAYER_UNKNOWN     ((AUD_LAYER_T) 0)    /**< Unknown layer */
#define         AUD_LAYER_1           ((AUD_LAYER_T) 1)    /**< Layer 1 */
#define         AUD_LAYER_2           ((AUD_LAYER_T) 2)    /**< Layer 2 */
#define         AUD_LAYER_3           ((AUD_LAYER_T) 3)    /**< Layer 3 */

/* Typedef and enumeration for the Audio type description. */
typedef  UINT8  AUD_TYPE_T;    /**< Audio type description. */

#define     AUD_TYPE_UNKNOWN          ((AUD_TYPE_T) 0)    /**< Unknown audio type */
#define     AUD_TYPE_CLEAN            ((AUD_TYPE_T) 1)    /**< Clean effects */
#define     AUD_TYPE_HEARING_IMPAIRED ((AUD_TYPE_T) 2)    /**< Hearing imparied */
#define     AUD_TYPE_VISUAL_IMPAIRED  ((AUD_TYPE_T) 3)    /**< Visual impaired commentary */
#define     AUD_TYPE_RESERVED         ((AUD_TYPE_T) 4)    /**< Reserved */
#define     AUD_TYPE_COMPLETE_MAIN    ((AUD_TYPE_T) 5)    /**   complete main */  
#define     AUD_TYPE_MUSIC_AND_EFFECT ((AUD_TYPE_T) 6)    /**   music and effect */
#define     AUD_TYPE_DIALOGUE         ((AUD_TYPE_T) 7)    /**   dialogue*/
#define     AUD_TYPE_COMMENTARY       ((AUD_TYPE_T) 8)    /**   commentary*/
#define     AUD_TYPE_EMERGENCY        ((AUD_TYPE_T) 9)    /**   emergency*/
#define     AUD_TYPE_VOICE_OVER       ((AUD_TYPE_T) 8)    /**   commentary*/
#define     AUD_TYPE_KARAOKE          ((AUD_TYPE_T) 9)    /**   emergency*/

/* Typedef and enumeration for the Audio profile and level description. */
typedef  UINT8  AUD_PROF_LVL_T;    /**< Audio profile and level description. */

#define     AUD_PROF_LVL_RESERVED      ((AUD_PROF_LVL_T) 0)     /**< Reserved */
#define     AUD_PROF_LVL_AAC_LV4       ((AUD_PROF_LVL_T) 0x52)  /**< AAC Level4 */
#define     AUD_PROF_LVL_HEAAC_LVL2    ((AUD_PROF_LVL_T) 0x58)  /**< HE-AAC Level 2 */
#define     AUD_PROF_LVL_HEAAC_LVL4    ((AUD_PROF_LVL_T) 0x5A)  /**< HE-AAC Level 4 */

/*Typedef and enumeration for the Audio PCM align*/
typedef  UINT8  AUD_PCM_ALIGN_T;    /**< Audio PCM align. */

#define     AUD_PCM_LITTLE_ENDIAN     ((AUD_PCM_ALIGN_T) 0)    /**< Little endian */
#define     AUD_PCM_BIG_ENDIAN        ((AUD_PCM_ALIGN_T) 1)    /**< Big endian */

typedef UINT8   AUD_MIX_TYPE_T;

#define AUD_MIX_UNKNOWN               ((AUD_MIX_TYPE_T) 0)
#define AUD_MIX_SUPPLEMENTARY         ((AUD_MIX_TYPE_T) 1)
#define AUD_MIX_INDEPENDENT           ((AUD_MIX_TYPE_T) 2)

typedef UINT8   AUD_CLASSIFICATION_T;

#define AUD_CLASS_RESERVED                              ((AUD_CLASSIFICATION_T) 0)
#define AUD_CLASS_MAIN                                  ((AUD_CLASSIFICATION_T) 1)
#define AUD_CLASS_VISUAL_IMPAIRED_AD                    ((AUD_CLASSIFICATION_T) 2)
#define AUD_CLASS_HEARING_IMPAIRED_CLEAN                ((AUD_CLASSIFICATION_T) 3)
#define AUD_CLASS_VISUAL_IMPAIRED_SPOKEN_SUBTITLE       ((AUD_CLASSIFICATION_T) 4)

/* Video stream property */
typedef  UINT8  VID_FMT_T;    /**< Video stream property. */

#define         VID_FMT_UNKNOWN       ((VID_FMT_T)  0)    /**< Unknown video format */

/*    Video High Definition   */
#define         VID_FMT_HD_4_3        ((VID_FMT_T)  1)    /**< HD 4:3 video */
#define         VID_FMT_HD_16_9       ((VID_FMT_T)  2)    /**< HD 16:9 video */

/*    Video Standard Definition. */
#define         VID_FMT_SD_4_3        ((VID_FMT_T)  3)    /**< SD 4:3 video */
#define         VID_FMT_SD_16_9       ((VID_FMT_T)  4)    /**< SD 16:9 video */

/* keep the following macro temporarily so we won't break the compiling
   process.
*/
#define         VID_FMT_VD_16_9       ((VID_FMT_T)  4)    /**< kept temporarily so we won't break the compiling process */

/* DSMCC type. */
typedef  UINT8  DSMCC_TYPE_T;    /**< DSMCC type. */

#define         DSMCC_TYPE_UNKNOWN    ((DSMCC_TYPE_T) 0)    /**< Unknown type */
#define         DSMCC_TYPE_A          ((DSMCC_TYPE_T) 1)    /**< Type A */
#define         DSMCC_TYPE_B          ((DSMCC_TYPE_T) 2)    /**< Type B */
#define         DSMCC_TYPE_C          ((DSMCC_TYPE_T) 3)    /**< Type C */
#define         DSMCC_TYPE_D          ((DSMCC_TYPE_T) 4)    /**< Type D */
#define         DSMCC_TYPE_SYNC       ((DSMCC_TYPE_T) 5)    /**< Type Sync */

#define         DSMCC_FLAG_CAROUSEL_ID      MAKE_BIT_MASK_32(0)    /**< Carousel ID is present */
#define         DSMCC_FLAG_ASSOCIATION_TAG  MAKE_BIT_MASK_32(1)    /**< Association tag is present */
#define         DSMCC_FLAG_USE              MAKE_BIT_MASK_32(2)    /**< Association tag use is present */

/* Private data format */
typedef  UINT32  PRIVATE_FMT_T;    /**< Private data format. */

#define          PRIVATE_FMT_UNKNOWN  ((PRIVATE_FMT_T) 0)    /**< Unknown format */
#define          PRIVATE_FMT_MHEG5    ((PRIVATE_FMT_T) 1)    /**< MHEG5 format */
#define          PRIVATE_FMT_MHP      ((PRIVATE_FMT_T) 2)    /**< MHP format */

/*
   MKV SubTitle Types
*/
typedef  UINT8   MKV_SBTL_TYPE_T;    /**< MKV SubTitle Types. */

#define          MKV_SBTL_IMAGE       ((MKV_SBTL_TYPE_T)1)    /**< Image subTitle type */
#define          MKV_SBTL_SRT         ((MKV_SBTL_TYPE_T)2)    /**< SRT subTitle type */
#define          MKV_SBTL_SSA         ((MKV_SBTL_TYPE_T)3)    /**< SSA subTitle type */

/* Dual mono indices (only when AUD_FMT_T is AUD_FMT_DUAL_MONO) */

#define AUDIO_INDEX_DUAL_0  0    /**< Dual mono index 0 */
#define AUDIO_INDEX_DUAL_1  1    /**< Dual mono index 1 */
#define AUDIO_INDEX_DUAL_MONO_FAKE_STEREO   2    /**< Dual mono fake stereo */
#define AUDIO_INDEX_DUAL_MONO_MIXED_MIXING  3    /**< Dual mono mixed mixing */


/* E-AC3 substream indices (only when AUD_FMT_T is AUD_FMT_DUAL_MONO) */

#define AUDIO_INDEX_SUBSTREAM_1  4    /**< Substream_1 index 4 */
#define AUDIO_INDEX_SUBSTREAM_2  5    /**< Substream_2 index 5 */
#define AUDIO_INDEX_SUBSTREAM_3  6    /**< Substream_3 index 6 */
#define AUDIO_INDEX_UNKNOW       7    /**< unknow index know */


/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
  following definition for color system is deprecated, use COLOR_SYS_T
  in u_common.h instead.
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&  */

typedef UINT8  VID_COLOR_SYS_T;    /**< Deprecated definition for color system, use COLOR_SYS_T instead */

#define VID_COLOR_SYS_UNKNOWN   ((VID_COLOR_SYS_T)  0)    /**< Unknown color system */
#define VID_COLOR_SYS_NTSC      ((VID_COLOR_SYS_T)  1)    /**< NTSC color system */
#define VID_COLOR_SYS_PAL       ((VID_COLOR_SYS_T)  2)    /**< PAL color system */
#define VID_COLOR_SYS_SECAM     ((VID_COLOR_SYS_T)  3)    /**< SECAM color system */


/* Null version id. */
#define SCDB_NULL_VER_ID  (CDB_NULL_VER_ID)    /**< NULL SCDB version ID used at the start of the iteration. */

/*---------------------------------------------------------------------------
 Structures
----------------------------------------------------------------------------*/

/********** Structure for common data *********/

/*------------------------------------------------------------------*/
/*! @brief  Structure for MPEG stream common data.
 *  @code
 *  typedef struct _SCDB_MPEG_COMMON_T
 *  {
 *      MPEG_2_PID_T      ui2_pcr;
 *  } SCDB_MPEG_COMMON_T;
 *  @endcode
 *  @li@c  ui2_pcr                       - PCR PID.
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_MPEG_COMMON_T
{
    MPEG_2_PID_T      ui2_pcr;
} SCDB_MPEG_COMMON_T;

/*
  Specify the number of UINT32 for the dummy buffers in the SCDB_COMMON_T
  structure.

*/
#define   N_UINT32_ELEM_CMN_1     2    /**< Specify the number of UINT32 for the dummy buffer 1 in the SCDB_COMMON_T structure. */
#define   N_UINT32_ELEM_CMN_2     3    /**< Specify the number of UINT32 for the dummy buffer 2 in the SCDB_COMMON_T structure. */

/*------------------------------------------------------------------*/
/*! @brief  Structure for SCDB record's common data.
 *  @code
 *  typedef struct _SCDB_COMMON_T
 *  {
 *      DATA_FMT_T      e_data_fmt;
 *      FEED_MODE_T     e_feed_mode;	
 *      UINT32          aui4_filler[N_UINT32_ELEM_CMN_1];
 *      union
 *      {
 *          SCDB_MPEG_COMMON_T    t_mpeg;
 *          UINT32                aui4_filler[N_UINT32_ELEM_CMN_2];
 *      } u_data;
 *  } SCDB_COMMON_T;
 *  @endcode
 *  @li@c  e_data_fmt                    - Data format.
 *  @li@c  e_feed_mode                   - Feed mode, push or pull etc.
 *  @li@c  aui4_filler                   - Dummy buffer to puff up the shared
 *                                         data to 8 bytes.
 *  @li@c  t_mpeg                        - MPEG transport specific parameters.
 *  @li@c  aui4_filler                   - Dummy buffer to puff up the union
 *                                         struct to 12 bytes.
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_COMMON_T
{
    DATA_FMT_T      e_data_fmt;
    FEED_MODE_T     e_feed_mode;	/* push or pull etc */

    /* Dummy buffer to puff up the shared data to 8 bytes. */
    UINT32          aui4_filler[N_UINT32_ELEM_CMN_1];

    /* Transport specific parameters. */
    union
    {
        SCDB_MPEG_COMMON_T    t_mpeg;

        /*
          Dummy buffer to puff up the union struct
          to 12 bytes.
        */
        UINT32                aui4_filler[N_UINT32_ELEM_CMN_2];
    } u_data;

} SCDB_COMMON_T;

/*
  Size of the common data for the record structure
*/
#define  SCDB_COMMON_DATA_SIZE (sizeof(SCDB_COMMON_T))    /**< Size of the common data for the record structure. */

/********* Structure for MPEG audio stream *********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for MPEG audio stream
 *  @code
 *  typedef struct _SCDB_AUDIO_MPEG_T
 *  {
 *      MPEG_2_PID_T   ui2_pid;
 *      AUD_FMT_T      e_aud_fmt;
 *      AUD_ENC_T      e_enc;
 *      AUD_LAYER_T    e_layer;
 *      AUD_TYPE_T     e_type;
 *      AUD_PROF_LVL_T e_prof_lvl;
 *      UINT8          ui1_index;
 *      UINT16         ui2_pmt_index;
 *      AUD_MIX_TYPE_T e_mix_type;
 *      AUD_CLASSIFICATION_T  e_class;
 *      CHAR           s_lang[ISO_639_LANG_LEN];
 *      BOOL           b_default;
 *  } SCDB_AUDIO_MPEG_T;
 *  @endcode
 *  @li@c  ui2_pid                       - PID
 *  @li@c  e_aud_fmt                     - Audio format
 *  @li@c  e_enc                         - Audio encoding
 *  @li@c  e_layer                       - Audio layer
 *  @li@c  e_type                        - Audio type
 *  @li@c  e_prof_lvl                    - Audio profile & level
 *  @li@c  ui1_index                     - Index
 *  @li@c  s_lang[ISO_639_LANG_LEN]      - Language
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_AUDIO_MPEG_T
{
    MPEG_2_PID_T   ui2_pid;
    AUD_FMT_T      e_aud_fmt;
    AUD_ENC_T      e_enc;
    AUD_LAYER_T    e_layer;
    AUD_TYPE_T     e_type;
    AUD_PROF_LVL_T e_prof_lvl;
    UINT8          ui1_index;
    UINT16         ui2_pmt_index;

    /* supplementary descriptor support*/
    AUD_MIX_TYPE_T e_mix_type;
    AUD_CLASSIFICATION_T  e_class;

    UINT8          ui1_component_tag;
    CHAR           s_lang[ISO_639_LANG_LEN];
    BOOL           b_default;
} SCDB_AUDIO_MPEG_T;


/*------------------------------------------------------------------*/
/*! @brief  Structure for H.264/AVC video descriptor
 *  @code
 *  typedef struct 	_AVC_VID_DESC_T
 *  {
 *      UINT8          ui1_level_idc;
 *  } AVC_VID_DESC_T;
 *  @endcode
 *  @li@c  ui1_level_idc                 - Level idc
 */
/*------------------------------------------------------------------*/
typedef struct 	_AVC_VID_DESC_T
{
#if 0 /* These fields are not used */
    UINT8          ui1_profile_idc;
    BOOL           b_constraint_set0_flag;
    BOOL           b_constraint_set1_flag;
    BOOL           b_constraint_set2_flag;
    UINT8          ui1_avc_compatible_flags; /* bottom 5 bits */
#endif
    UINT8          ui1_level_idc;
#if 0 /* These fields are not used */
    BOOL           b_avc_still_present;
    BOOL           b_avc_24_hour_picture_flag;
#endif
} AVC_VID_DESC_T;

/* for 3D video */
typedef UINT8 REALD_LAYER_TYPE_T;

#define REALD_UNKNOWN_LAYER     ((REALD_LAYER_TYPE_T)0x0)
#define REALD_BASE_LAYER        ((REALD_LAYER_TYPE_T)0x1)
#define REALD_ENHANCEMENT_LAYER ((REALD_LAYER_TYPE_T)0x2)


typedef UINT8 REALD_CODING_METHOD_T;

#define CRITICALLY_SAMPLED_METHOD        ((REALD_CODING_METHOD_T)0x00)
#define NON_CRITICALLY_SAMPLED_METHOD    ((REALD_CODING_METHOD_T)0x80)

/* RD3D_registration descriptor */
typedef struct _RD3D_REG_DESC_T
{
    BOOL b_rd3d;
    REALD_LAYER_TYPE_T e_layer_type;
    REALD_CODING_METHOD_T e_coding_method;    
} RD3D_REG_DESC_T;


/* for mvc source */
typedef UINT8 MVC_SRC_TYPE_T;/* from STREAM_TYPE_MVC or Tag*/

#define MVC_SRC_TYPE_UNKNOWN    ((MVC_SRC_TYPE_T)0x0)
#define MVC_SRC_TYPE_STRM_MVC   ((MVC_SRC_TYPE_T)0x1)
#define MVC_SRC_TYPE_STRM_TAG   ((MVC_SRC_TYPE_T)0x2)

/* MVC extension descriptor */
typedef struct _MVC_EXTENSION_DESC_T
{
    BOOL b_mvc_video;
    BOOL b_mvc_ext_desc;
    UINT16 ui2_avg_bit_rate;
    UINT16 ui2_max_bit_rate;
    UINT16 ui2_view_ord_idx_min;
    UINT16 ui2_view_ord_idx_max;
    UINT8  ui1_temporal_id_start;
    UINT8  ui1_temporal_id_end;
    BOOL b_no_sei_nal_present;
    BOOL b_no_prefix_nal_present;
    MVC_SRC_TYPE_T      e_mvc_src;
} MVC_EXTENSION_DESC_T;

/********* Structure for MPEG video stream *********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for MPEG video stream
 *  @code
 *  typedef struct 	_SCDB_VIDEO_MPEG_T
 *  {
 *      MPEG_2_PID_T   ui2_pid;
 *      VID_ENC_T      e_enc;
 *      VID_FMT_T      e_vid_fmt;
 *      BOOL           b_default;
 *      AVC_VID_DESC_T t_avc_vid_desc;
 *  } SCDB_VIDEO_MPEG_T;
 *  @endcode
 *  @li@c  ui2_pid                       - PID
 *  @li@c  e_enc                         - Encoding
 *  @li@c  e_vid_fmt                     - Video format
 *  @li@c  b_default                     - Primary stream indicator
 *  @li@c  t_avc_vid_desc                - AVC video descriptor
 */
/*------------------------------------------------------------------*/
typedef struct 	_SCDB_VIDEO_MPEG_T
{
    MPEG_2_PID_T   ui2_pid;
    VID_ENC_T      e_enc;
    VID_FMT_T      e_vid_fmt;
    BOOL           b_default;
    AVC_VID_DESC_T t_avc_vid_desc;
    UINT8          ui1_component_tag;
    UINT16          ui2_pmt_index;

    RD3D_REG_DESC_T         t_rd3d;
    MVC_EXTENSION_DESC_T    t_mvc_ext;
} SCDB_VIDEO_MPEG_T;

/********* Structure for MPEG private data stream *********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for MPEG private data stream
 *  @code
 *  typedef struct  _SCDB_PRIVATE_MPEG_T
 *  {
 *      MPEG_2_PID_T               ui2_pid;
 *      PRIVATE_FMT_T              e_private_fmt;
 *  } SCDB_PRIVATE_MPEG_T;
 *  @endcode
 *  @li@c  ui2_pid                       - PID
 *  @li@c  e_private_fmt                 - Private data format
 */
/*------------------------------------------------------------------*/
typedef struct  _SCDB_PRIVATE_MPEG_T
{
    MPEG_2_PID_T               ui2_pid;
    PRIVATE_FMT_T              e_private_fmt;
} SCDB_PRIVATE_MPEG_T;

/********* Structure for MPEG DSMCC stream *********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for MPEG DSMCC stream
 *  @code
 *  typedef struct  _SCDB_DSMCC_MPEG_T
 *  {
 *      MPEG_2_PID_T               ui2_pid;
 *      DSMCC_TYPE_T               e_type;
 *      UINT32                     ui4_flags;
 *      UINT32                     ui4_carousel_id;
 *      UINT16                     ui2_association_tag;
 *      UINT16                     ui2_use;
 *      BOOL                       b_default;
 *  }  SCDB_DSMCC_MPEG_T;
 *  @endcode
 *  @li@c  ui2_pid                       - PID
 *  @li@c  e_type                        - Type
 *  @li@c  ui4_flags                     - Flags
 *  @li@c  ui4_carousel_id               - Carousel ID, if present
 *  @li@c  ui2_association_tag           - Association tag, if present
 *  @li@c  ui2_use                       - Use, if present
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct  _SCDB_DSMCC_MPEG_T
{
    MPEG_2_PID_T               ui2_pid;
    DSMCC_TYPE_T               e_type;
    UINT32                     ui4_flags;
    UINT32                     ui4_carousel_id;
    UINT16                     ui2_association_tag;
    UINT16                     ui2_use;
    BOOL                       b_default;
}  SCDB_DSMCC_MPEG_T;

/********* Structures for Close-Caption stream *******/
/*------------------------------------------------------------------*/
/*! @brief  Structures for ATSC Line 21 Closed Caption stream
 *  @code
 *  typedef struct _CC_ATSC_LINE_21_T
 *  {
 *      BOOL  b_line_21_field;
 *      UINT8 ui1_cc_id;
 *      BOOL  b_easy_reader;
 *      BOOL  b_wide_aspect;
 *      BOOL  b_det_in_descr;
 *  } CC_ATSC_LINE_21_T;
 *  @endcode
 *  @li@c  b_line_21_field               - Line 21 field 2(true)/1(false) indicator
 *  @li@c  ui1_cc_id                     - Closed Caption ID
 *  @li@c  b_easy_reader                 - Indicates whether caption text is tailored to the needs of beginning readers
 *  @li@c  b_wide_aspect                 - Wide (16:9, true) / Normal (4:3, false) aspect ratio indicator
 *  @li@c  b_det_in_descr                - Detected in descriptor indicator
 */
/*------------------------------------------------------------------*/
typedef struct _CC_ATSC_LINE_21_T
{
    BOOL  b_line_21_field;  /* This will be removed.*/
    UINT8 ui1_cc_id;
    BOOL  b_easy_reader;
    BOOL  b_wide_aspect;
    BOOL  b_det_in_descr;
} CC_ATSC_LINE_21_T;

/*------------------------------------------------------------------*/
/*! @brief  Structures for ATSC DTV Closed Caption stream
 *  @code
 *  typedef struct _CC_ATSC_DTV_T
 *  {
 *      UINT8  ui1_caption_svc_num;
 *      BOOL  b_easy_reader;
 *      BOOL  b_wide_aspect;
 *  } CC_ATSC_DTV_T;
 *  @endcode
 *  @li@c  ui1_caption_svc_num           - Service Number (6-bit) assocated with the CC stream
 *  @li@c  b_easy_reader                 - Indicates whether caption text is tailored to the needs of beginning readers
 *  @li@c  b_wide_aspect                 - Wide (16:9, true) / Normal (4:3, false) aspect ratio indicator
 */
/*------------------------------------------------------------------*/
typedef struct _CC_ATSC_DTV_T
{
    UINT8  ui1_caption_svc_num;
    BOOL  b_easy_reader;
    BOOL  b_wide_aspect;
} CC_ATSC_DTV_T;

/*------------------------------------------------------------------*/
/*! @brief  Structures for Closed Caption stream
 *  @code
 *  typedef struct _SCDB_CLOSED_CAPTION_T
 *  {
 *      CC_TYPE_T  e_type;
 *      CHAR  s_lang [ISO_639_LANG_LEN];
 *      BOOL  b_default;
 *      union
 *      {
 *          CC_ATSC_LINE_21_T      t_cc_atsc_line_21;
 *          CC_ATSC_DTV_T          t_cc_atsc_dtv;
 *      }   udata;
 *  }   SCDB_CLOSED_CAPTION_T;
 *  @endcode
 *  @li@c  e_type                        - CC type
 *  @li@c  s_lang [ISO_639_LANG_LEN]     - Language
 *  @li@c  b_default                     - Primary stream indicator
 *  @li@c  t_cc_atsc_line_21             - Line-21 CC data
 *  @li@c  t_cc_atsc_dtv                 - DTVCC data
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_CLOSED_CAPTION_T
{
    CC_TYPE_T  e_type;

    CHAR  s_lang [ISO_639_LANG_LEN];

    BOOL  b_default;

    union
    {
        CC_ATSC_LINE_21_T      t_cc_atsc_line_21;
        CC_ATSC_DTV_T          t_cc_atsc_dtv;
    }   udata;
}   SCDB_CLOSED_CAPTION_T;

/********* Structures for ISDB-Caption stream *******/
/*------------------------------------------------------------------*/
/*! @brief  Structures for ISDB Closed Caption stream
 *  @code
 *  typedef struct _SCDB_ISDB_CAPTION_T
 *  {
 *      MPEG_2_PID_T    ui2_pid;
 *      UINT8           ui1_caption_tag;
 *  }   SCDB_ISDB_CAPTION_T;
 *  @endcode
 *  @li@c  ui2_pid                       - PID
 *  @li@c  ui1_caption_tag               - Caption tag
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_ISDB_CAPTION_T
{
    MPEG_2_PID_T    ui2_pid;
    UINT8           ui1_caption_tag;
}   SCDB_ISDB_CAPTION_T;


/********* Structures for ISDB-Text stream *******/
/*------------------------------------------------------------------*/
/*! @brief  Structures for ISDB Superimpose stream
 *  @code
 *  typedef struct _SCDB_ISDB_TEXT_T
 *  {
 *      MPEG_2_PID_T    ui2_pid;
 *      UINT8           ui1_text_tag;
 *  }   SCDB_ISDB_TEXT_T;
 *  @endcode
 *  @li@c  ui2_pid                       - PID
 *  @li@c  ui1_text_tag                  - Superimpose tag
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_ISDB_TEXT_T
{
    MPEG_2_PID_T    ui2_pid;
    UINT8           ui1_text_tag;
}   SCDB_ISDB_TEXT_T;

/* Structure for DVB Subtitle stream   */
/*------------------------------------------------------------------*/
/*! @brief  Structure for DVB Subtitle stream
 *  @code
 *  typedef struct _SCDB_SUBTITLE_DVB_T
 *  {
 *      MPEG_2_PID_T               ui2_pid;
 *      UINT16                     ui2_comp_pg_id;
 *      UINT16                     ui2_anci_pg_id;
 *      CHAR                       s_lang [ISO_639_LANG_LEN];
 *      DVB_SBTL_TYPE_T            e_sbtl_type;
 *      UINT16                     ui2_pmt_index;
 *  }  SCDB_SUBTITLE_DVB_T;
 *  @endcode
 *  @li@c  ui2_pid                       - PID
 *  @li@c  ui2_comp_pg_id                - Comp page ID
 *  @li@c  ui2_anci_pg_id                - Anci page ID
 *  @li@c  s_lang [ISO_639_LANG_LEN]     - Language
 *  @li@c  e_sbtl_type                   - Subtitle type
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_SUBTITLE_DVB_T
{
    MPEG_2_PID_T               ui2_pid;
    UINT16                     ui2_comp_pg_id;
    UINT16                     ui2_anci_pg_id;
    CHAR                       s_lang [ISO_639_LANG_LEN];
    DVB_SBTL_TYPE_T            e_sbtl_type;
    UINT16                     ui2_pmt_index;
}  SCDB_SUBTITLE_DVB_T;

/* Structure for DVB's TELETEXT descriptor */
/*------------------------------------------------------------------*/
/*! @brief  Structure for DVB Teletext stream
 *  @code
 *  typedef struct _SCDB_TELETEXT_DVB_T
 *  {
 *      MPEG_2_PID_T               ui2_pid;
 *      BOOL                       b_page_info;
 *      DVB_TLTXT_TYPE_T           e_type;
 *      UINT8                      ui1_mag_num;
 *      UINT8                      ui1_page_num;
 *      BOOL                       b_default;
 *      CHAR                       s_lang [ISO_639_LANG_LEN];
 *      UINT16                     ui2_pmt_index;
 *  }  SCDB_TELETEXT_DVB_T;
 *  @endcode
 *  @li@c  ui2_pid                       - PID
 *  @li@c  b_page_info                   - Page info valid indicator
 *  @li@c  e_type                        - Teletext type
 *  @li@c  ui1_mag_num                   - Magazine number
 *  @li@c  ui1_page_num                  - Page number
 *  @li@c  b_default                     - Primary stream indicator
 *  @li@c  s_lang [ISO_639_LANG_LEN]     - Language
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_TELETEXT_DVB_T
{
    MPEG_2_PID_T               ui2_pid;
    BOOL                       b_page_info;
    DVB_TLTXT_TYPE_T           e_type;
    UINT8                      ui1_mag_num;
    UINT8                      ui1_page_num;
    BOOL                       b_default;
    CHAR                       s_lang [ISO_639_LANG_LEN];
    UINT16                     ui2_pmt_index;
}  SCDB_TELETEXT_DVB_T;

/* Structure for  DVB's VBI Teletext descriptor */
/*------------------------------------------------------------------*/
/*! @brief  Structure for DVB VBI Teletext stream
 *  @code
 *  typedef struct _SCDB_VBI_TELETEXT_DVB_T
 *  {
 *      MPEG_2_PID_T               ui2_pid;
 *      BOOL                       b_page_info;
 *      DVB_TLTXT_TYPE_T           e_type;
 *      UINT8                      ui1_mag_num;
 *      UINT8                      ui1_page_num;
 *      BOOL                       b_default;
 *      CHAR                       s_lang [ISO_639_LANG_LEN];
 *      UINT16                     ui2_pmt_index;
 *  }  SCDB_VBI_TELETEXT_DVB_T;
 *  @endcode
 *  @li@c  ui2_pid                       - PID
 *  @li@c  b_page_info                   - Page info valid indicator
 *  @li@c  e_type                        - Teletext type
 *  @li@c  ui1_mag_num                   - Magazine number
 *  @li@c  ui1_page_num                  - Page number
 *  @li@c  b_default                     - Primary stream indicator
 *  @li@c  s_lang [ISO_639_LANG_LEN]     - Language
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_VBI_TELETEXT_DVB_T
{
    MPEG_2_PID_T               ui2_pid;
    BOOL                       b_page_info;
    DVB_TLTXT_TYPE_T           e_type;
    UINT8                      ui1_mag_num;
    UINT8                      ui1_page_num;
    BOOL                       b_default;
    CHAR                       s_lang [ISO_639_LANG_LEN];
    UINT16                     ui2_pmt_index;
}  SCDB_VBI_TELETEXT_DVB_T;

/* Structure for  DVB's VBI Data stream descriptor */
/*------------------------------------------------------------------*/
/*! @brief  Structure for DVB VBI Data stream
 *  @code
 *  typedef struct _SCDB_VBI_DATA_DVB_T
 *  {
 *      MPEG_2_PID_T               ui2_pid;
 *      DVB_VBI_DATA_SERVICE_ID_T  e_service_ids;
 *  }  SCDB_VBI_DATA_DVB_T;
 *  @endcode
 *  @li@c  ui2_pid                       - PID
 *  @li@c  e_service_ids                 - VBI data service ID
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_VBI_DATA_DVB_T
{
    MPEG_2_PID_T               ui2_pid;
    DVB_VBI_DATA_SERVICE_ID_T  e_service_ids;  /* bitmask containing service id. */
}  SCDB_VBI_DATA_DVB_T;

/*------------------------------------------------------------------*/
/*! @brief  Structure for analog audio stream
 *  @code
 *  typedef struct _SCDB_AUDIO_ANALOG_T
 *  {
 *      UINT32                     ui4_tv_sys;
 *      UINT32                     ui4_audio_sys;
 *      AUD_FMT_T                  e_aud_fmt;
 *      AUD_ENC_T                  e_enc;
 *      BOOL                       b_default;
 *  }   SCDB_AUDIO_ANALOG_T;
 *  @endcode
 *  @li@c  ui4_tv_sys                    - TV system
 *  @li@c  ui4_audio_sys                 - Audio system
 *  @li@c  e_aud_fmt                     - Audio format
 *  @li@c  e_enc                         - Audio encoding
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_AUDIO_ANALOG_T
{
    UINT32                     ui4_tv_sys;
    UINT32                     ui4_audio_sys;
    AUD_FMT_T                  e_aud_fmt;
    AUD_ENC_T                  e_enc;
    BOOL                       b_default;
    BOOL                       b_enable_aud_sys_auto_detect;
}   SCDB_AUDIO_ANALOG_T;

/*------------------------------------------------------------------*/
/*! @brief  Structure for analog video stream
 *  @code
 *  typedef struct _SCDB_VIDEO_ANALOG_T
 *  {
 *      VID_FMT_T                  e_vid_fmt;
 *      COLOR_SYS_T                e_color_sys;
 *      BOOL                       b_default;
 *      BOOL                       b_detected;
 *  }   SCDB_VIDEO_ANALOG_T;
 *  @endcode
 *  @li@c  e_vid_fmt                     - Video format
 *  @li@c  e_color_sys                   - Color system
 *  @li@c  b_default                     - Primary stream indicator
 *  @li@c  b_detected                    - Specifies whether the video stream is now detected
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_VIDEO_ANALOG_T
{
    VID_FMT_T                  e_vid_fmt;
    COLOR_SYS_T                e_color_sys;
    BOOL                       b_default;
    BOOL                       b_detected;
}   SCDB_VIDEO_ANALOG_T;

/********* Structure for digital audio stream *********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for digital audio stream
 *  @code
 *  typedef struct _SCDB_AUDIO_DIGITAL_T
 *  {
 *      AUD_FMT_T    e_aud_fmt;
 *      AUD_ENC_T    e_enc;
 *      AUD_LAYER_T  e_layer;
 *      BOOL         b_default;
 *  } SCDB_AUDIO_DIGITAL_T;
 *  @endcode
 *  @li@c  e_aud_fmt                     - Audio format
 *  @li@c  e_enc                         - Audio encoding
 *  @li@c  e_layer                       - Audio layer
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_AUDIO_DIGITAL_T
{
    AUD_FMT_T    e_aud_fmt;
    AUD_ENC_T    e_enc;
    AUD_LAYER_T  e_layer;
    BOOL         b_default;
} SCDB_AUDIO_DIGITAL_T;

/********* Structure for AVI audio stream ***********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for AVI audio stream
 *  @code
 *  typedef struct _SCDB_AUDIO_AVI_T
 *  {
 *      UINT32         ui4_stream_id;
 *      UINT32         ui4_scale;
 *      UINT32         ui4_rate;
 *      AUD_ENC_T      e_enc;
 *      AUD_LAYER_T    e_layer;
 *      UINT32         ui4_sample_size;
 *      INT16          i2_channels;
 *      INT32          i4_samples_per_sec;
 *      INT16          i2_block_align;
 *      INT16          i2_bits_per_sample;
 *      CHAR           s_lang[ISO_639_LANG_LEN];
 *      BOOL           b_default;
 *  } SCDB_AUDIO_AVI_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  ui4_scale                     - Time scale
 *  @li@c  ui4_rate                      - Sample rate
 *  @li@c  e_enc                         - Audio encoding
 *  @li@c  e_layer                       - Audio layer
 *  @li@c  ui4_sample_size               - Sample size
 *  @li@c  i2_channels                   - Number of channels
 *  @li@c  i4_samples_per_sec            - Samples per second
 *  @li@c  i2_block_align                - Block alignment
 *  @li@c  i2_bits_per_sample            - Bits per sample
 *  @li@c  s_lang[ISO_639_LANG_LEN]      - Language
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_AUDIO_AVI_T
{
    UINT32         ui4_stream_id;
    UINT32         ui4_scale;
    UINT32         ui4_rate;
    AUD_ENC_T      e_enc;
    AUD_LAYER_T    e_layer;
    UINT32         ui4_sample_size;
    INT16          i2_channels;
    INT32          i4_samples_per_sec;
    INT16          i2_block_align;
    INT16          i2_bits_per_sample;
    CHAR           s_lang[ISO_639_LANG_LEN];
    BOOL           b_default;
} SCDB_AUDIO_AVI_T;

/********** Structure for AVI video stream ***********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for AVI video stream
 *  @code
 *  typedef struct         _SCDB_VIDEO_AVI_T
 *  {
 *      UINT32         ui4_stream_id;
 *      UINT32         ui4_scale;
 *      UINT32         ui4_rate;
 *      VID_ENC_T      e_enc;
 *      UINT32         ui4_sample_size;
 *      INT32          i4_width;
 *      INT32          i4_height;
 *      BOOL           b_default;
 *  } SCDB_VIDEO_AVI_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  ui4_scale                     - Time scale
 *  @li@c  ui4_rate                      - Sample rate
 *  @li@c  e_enc                         - Video encoding
 *  @li@c  ui4_sample_size               - Sample size
 *  @li@c  i4_width                      - Video width
 *  @li@c  i4_height                     - Video height
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct         _SCDB_VIDEO_AVI_T
{
    UINT32         ui4_stream_id;
    UINT32         ui4_scale;
    UINT32         ui4_rate;
    VID_ENC_T      e_enc;
    UINT32         ui4_sample_size;
    INT32          i4_width;
    INT32          i4_height;
    BOOL           b_default;
} SCDB_VIDEO_AVI_T;

/*********** Structure for DIVX Subtitle ************/
#define AVI_DIVX_SUBTITLE_TYPE_TEXT     1    /**< DivX subtitle of text type */
#define AVI_DIVX_SUBTITLE_TYPE_BITMAP   2    /**< DivX subtitle of bitmap type */

/*------------------------------------------------------------------*/
/*! @brief  Structure for DivX subtitle
 *  @code
 *  typedef struct         _SCDB_SUBTITLE_DIVX_T
 *  {
 *      UINT32         ui4_stream_id;
 *      UINT32         ui4_scale;
 *      UINT32         ui4_rate;
 *      UINT32         ui4_subtitle_type;
 *      UINT32         ui4_sample_size;
 *      INT32          i4_width;
 *      INT32          i4_height;
 *      CHAR           s_lang[ISO_639_LANG_LEN];
 *      BOOL           b_default;
 *  } SCDB_SUBTITLE_DIVX_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  ui4_scale                     - Time scale
 *  @li@c  ui4_rate                      - Sample rate
 *  @li@c  ui4_subtitle_type             - Subtitle type
 *  @li@c  ui4_sample_size               - Sample size
 *  @li@c  i4_width                      - Canvas width
 *  @li@c  i4_height                     - Canvas height
 *  @li@c  s_lang[ISO_639_LANG_LEN]      - Language
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct         _SCDB_SUBTITLE_DIVX_T
{
    UINT32         ui4_stream_id;
    UINT32         ui4_scale;
    UINT32         ui4_rate;
    UINT32         ui4_subtitle_type;
    UINT32         ui4_sample_size;
    INT32          i4_width;
    INT32          i4_height;
    CHAR           s_lang[ISO_639_LANG_LEN];
    BOOL           b_default;
} SCDB_SUBTITLE_DIVX_T;

/*********** Structure for MPEG-4 audio ************/
/*------------------------------------------------------------------*/
/*! @brief  Structure for MPEG-4 audio
 *  @code
 *  typedef struct _SCDB_AUDIO_MP4_T
 *  {
 *      UINT32         ui4_stream_id;
 *      AUD_ENC_T      e_enc;
 *      AUD_LAYER_T    e_layer;
 *      INT16          i2_channels;
 *      INT32          i4_sample_rate;
 *      INT32          i4_sample_size;
 *      BOOL           b_default;
 *  } SCDB_AUDIO_MP4_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  e_enc                         - Audio encoding
 *  @li@c  e_layer                       - Audio layer
 *  @li@c  i2_channels                   - Number of channels
 *  @li@c  i4_sample_rate                - Sample rate
 *  @li@c  i4_sample_size                - Sample size
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_AUDIO_MP4_T
{
    UINT32         ui4_stream_id;
    AUD_ENC_T      e_enc;
    AUD_LAYER_T    e_layer;
    INT16          i2_channels;
    INT32          i4_sample_rate;
    INT32          i4_sample_size;
    BOOL           b_default;
} SCDB_AUDIO_MP4_T;

typedef struct _SCDB_SUBTITLE_MP4_T
{
    UINT32         ui4_stream_id;
    INT32          i4_width;
    INT32          i4_height;
    
} SCDB_SUBTITLE_MP4_T;

/*********** Structure for MPEG-4 video ************/
/*------------------------------------------------------------------*/
/*! @brief  Structure for MPEG-4 video
 *  @code
 *  typedef struct _SCDB_VIDEO_MP4_T
 *  {
 *      UINT32         ui4_stream_id;
 *      VID_ENC_T      e_enc;
 *      VID_FMT_T      e_vid_fmt;
 *      INT32          i4_width;
 *      INT32          i4_height;
 *      BOOL           b_default;
 *  } SCDB_VIDEO_MP4_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  e_enc                         - Video encoding
 *  @li@c  e_vid_fmt                     - Video format
 *  @li@c  i4_width                      - Video width
 *  @li@c  i4_height                     - Video height
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_VIDEO_MP4_T
{
    UINT32         ui4_stream_id;
    VID_ENC_T      e_enc;
    VID_FMT_T      e_vid_fmt;
    INT32          i4_width;
    INT32          i4_height;
    BOOL           b_default;
} SCDB_VIDEO_MP4_T;

/********** Structure for ASF Audio **************/
/*------------------------------------------------------------------*/
/*! @brief  Structure for ASF audio
 *  @code
 *  typedef struct _SCDB_AUDIO_ASF_T
 *  {
 *      UINT32         ui4_stream_id;
 *      AUD_ENC_T      e_enc;
 *      AUD_LAYER_T    e_layer;
 *      INT16          i2_channels;
 *      INT32          i4_samples_per_sec;
 *      INT16          i2_block_align;
 *      INT16          i2_bits_per_sample;
 *      CHAR           s_lang[ISO_639_LANG_LEN];
 *      BOOL           b_default;
 *  } SCDB_AUDIO_ASF_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  e_enc                         - Audio encoding
 *  @li@c  e_layer                       - Audio layer
 *  @li@c  i2_channels                   - Number of channels
 *  @li@c  i4_samples_per_sec            - Sample per second
 *  @li@c  i2_block_align                - Block alignment
 *  @li@c  i2_bits_per_sample            - Bits per sample
 *  @li@c  s_lang[ISO_639_LANG_LEN]      - Language
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_AUDIO_ASF_T
{
    UINT32         ui4_stream_id;
    AUD_ENC_T      e_enc;
    AUD_LAYER_T    e_layer;
    INT16          i2_channels;
    INT32          i4_samples_per_sec;
    INT16          i2_block_align;
    INT16          i2_bits_per_sample;
    CHAR           s_lang[ISO_639_LANG_LEN];
    BOOL           b_default;
} SCDB_AUDIO_ASF_T;

/********** Structure for ASF Video **************/
/*------------------------------------------------------------------*/
/*! @brief  Structure for ASF video
 *  @code
 *  typedef struct         _SCDB_VIDEO_ASF_T
 *  {
 *      UINT32         ui4_stream_id;
 *      VID_ENC_T      e_enc;
 *      INT32          i4_width;
 *      INT32          i4_height;
 *      BOOL           b_default;
 *  } SCDB_VIDEO_ASF_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  e_enc                         - Video encoding
 *  @li@c  i4_width                      - Video width
 *  @li@c  i4_height                     - Video height
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct         _SCDB_VIDEO_ASF_T
{
    UINT32         ui4_stream_id;
    VID_ENC_T      e_enc;
    INT32          i4_width;
    INT32          i4_height;
    BOOL           b_default;
} SCDB_VIDEO_ASF_T;

/********** Structure for MPEG Audio program stream **********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for MPEG PS audio program stream
 *  @code
 *  typedef struct _SCDB_AUDIO_PS_T
 *  {
 *      UINT8          ui1_stream_id;
 *      UINT8          ui1_sub_stream_id;
 *      AUD_ENC_T      e_enc;
 *      AUD_LAYER_T    e_layer;
 *      INT32          i4_samples_per_quant;
 *      INT32          i4_sampling_frequency;
 *      INT16          i2_channels;
 *      BOOL           b_default;
 *  } SCDB_AUDIO_PS_T;
 *  @endcode
 *  @li@c  ui1_stream_id                 - Stream ID
 *  @li@c  ui1_sub_stream_id             - Sub stream ID
 *  @li@c  e_enc                         - Audio encoding
 *  @li@c  e_layer                       - Audio layer
 *  @li@c  i4_samples_per_quant          - Sample per quant
 *  @li@c  i4_sampling_frequency         - Sampling frequency
 *  @li@c  i2_channels                   - Number of channels
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_AUDIO_PS_T
{
    UINT8          ui1_stream_id;
    UINT8          ui1_sub_stream_id;
    AUD_ENC_T      e_enc;
    AUD_LAYER_T    e_layer;
    INT32          i4_samples_per_quant;
    INT32          i4_sampling_frequency;
    INT16          i2_channels;
    BOOL           b_default;
} SCDB_AUDIO_PS_T;

/********** Structure for MPEG Video program stream **********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for MPEG PS video program stream
 *  @code
 *  typedef struct         _SCDB_VIDEO_PS_T
 *  {
 *      UINT8          ui1_stream_id;
 *      VID_ENC_T      e_enc;
 *      VID_FMT_T      e_vid_fmt;
 *      INT32          i4_width;
 *      INT32          i4_height;
 *      BOOL           b_default;
 *  } SCDB_VIDEO_PS_T;
 *  @endcode
 *  @li@c  ui1_stream_id                 - Stream ID
 *  @li@c  e_enc                         - Video encoding
 *  @li@c  e_vid_fmt                     - Video format
 *  @li@c  i4_width                      - Video width
 *  @li@c  i4_height                     - Video height
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct         _SCDB_VIDEO_PS_T
{
    UINT8          ui1_stream_id;
    VID_ENC_T      e_enc;
    VID_FMT_T      e_vid_fmt;
    INT32          i4_width;
    INT32          i4_height;
    BOOL           b_default;
} SCDB_VIDEO_PS_T;

/********** Structure for MKV video stream ***********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for MKV video stream
 *  @code
 *  typedef struct _SCDB_VIDEO_MKV_T
 *  {
 *      UINT32         ui4_stream_id;
 *      UINT32         ui4_scale;
 *      UINT32         ui4_rate;
 *      VID_ENC_T      e_enc;
 *      UINT32         ui4_sample_size;
 *      INT32          i4_width;
 *      INT32          i4_height;
 *      BOOL           b_default;
 *  } SCDB_VIDEO_MKV_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  ui4_scale                     - Time scale
 *  @li@c  ui4_rate                      - Sample rate
 *  @li@c  e_enc                         - Video encoding
 *  @li@c  ui4_sample_size               - Sample size
 *  @li@c  i4_width                      - Video width
 *  @li@c  i4_height                     - Video height
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_VIDEO_MKV_T
{
    UINT32         ui4_stream_id;
    UINT32         ui4_scale;
    UINT32         ui4_rate;
    VID_ENC_T      e_enc;
    UINT32         ui4_sample_size;
    INT32          i4_width;
    INT32          i4_height;
    BOOL           b_default;
} SCDB_VIDEO_MKV_T;

/********* Structure for MKV audio stream ***********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for MKV audio stream
 *  @code
 *  typedef struct _SCDB_AUDIO_MKV_T
 *  {
 *      UINT32         ui4_stream_id;
 *      UINT32         ui4_scale;
 *      UINT32         ui4_rate;
 *      AUD_ENC_T      e_enc;
 *      AUD_LAYER_T    e_layer;
 *      AUD_PCM_ALIGN_T   e_pcm_align;
 *      UINT32         ui4_sample_size;
 *      INT16          i2_channels;
 *      INT32          i4_samples_per_sec;
 *      INT16          i2_block_align;
 *      INT16          i2_bits_per_sample;
 *      CHAR           s_lang[ISO_639_LANG_LEN];
 *      BOOL           b_default;
 *  } SCDB_AUDIO_MKV_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  ui4_scale                     - Time scale
 *  @li@c  ui4_rate                      - Sample rate
 *  @li@c  e_enc                         - Audio encoding
 *  @li@c  e_layer                       - Audio layer
 *  @li@c  e_pcm_align                   - PCM alignment
 *  @li@c  ui4_sample_size               - Sample size
 *  @li@c  i2_channels                   - Number of channels
 *  @li@c  i4_samples_per_sec            - Samples per second
 *  @li@c  i2_block_align                - Block alignment
 *  @li@c  i2_bits_per_sample            - Bits per sample
 *  @li@c  s_lang[ISO_639_LANG_LEN]      - Language
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_AUDIO_MKV_T
{
    UINT32         ui4_stream_id;
    UINT32         ui4_scale;
    UINT32         ui4_rate;
    AUD_ENC_T      e_enc;
    AUD_LAYER_T    e_layer;
    AUD_PCM_ALIGN_T   e_pcm_align;
    UINT32         ui4_sample_size;
    INT16          i2_channels;
    INT32          i4_samples_per_sec;
    INT16          i2_block_align;
    INT16          i2_bits_per_sample;
    CHAR           s_lang[ISO_639_LANG_LEN];
    BOOL           b_default;
} SCDB_AUDIO_MKV_T;

/********** Structure for RM video stream ***********/
/*------------------------------------------------------------------*/
/*! @brief  Structurefor RM video stream
 *  @code
 *  typedef struct _SCDB_VIDEO_RM_T
 *  {
 *      UINT32         ui4_stream_id;
 *      VID_ENC_T      e_enc;
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
 *  } SCDB_VIDEO_RM_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  e_enc                         - Video encoding
 *  @li@c  ui4_preroll                   - Preroll
 *  @li@c  ui4_duration                  - Duration
 *  @li@c  ui4_max_bit_rate              - Maximum bit rate
 *  @li@c  ui4_avg_bit_rate              - Average bit rate
 *  @li@c  ui4_max_packet_size           - Maximum packet size
 *  @li@c  ui4_avg_packet_size           - Average packet size
 *  @li@c  ui4_start_time                - Start time
 *  @li@c  ui4_codec_id                  - Codec ID
 *  @li@c  ui2_frm_width                 - Frame width
 *  @li@c  ui2_frm_height                - Frame height
 *  @li@c  ui2_bit_count                 - Bit count
 *  @li@c  ui2_pad_width                 - Pad width
 *  @li@c  ui2_pad_height                - Pad height
 *  @li@c  ui4_frm_rate                  - Frame rate
 *  @li@c  ui4_spo_extra_flags           - Spo extra flags
 *  @li@c  ui4_version                   - Version
 *  @li@c  ui4_specific                  - Specific
 *  @li@c  ui1_ecc_mask                  - Ecc mask
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_VIDEO_RM_T
{
    UINT32         ui4_stream_id;
    VID_ENC_T      e_enc;
    UINT32  ui4_preroll;
    UINT32  ui4_duration;
    UINT32  ui4_max_bit_rate;
    UINT32  ui4_avg_bit_rate;
    UINT32  ui4_max_packet_size;
    UINT32  ui4_avg_packet_size;
    UINT32  ui4_start_time;
    UINT32  ui4_codec_id;/*e_enc*/
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
} SCDB_VIDEO_RM_T;

/********* Structure for RM audio stream ***********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for RM audio stream
 *  @code
 *  typedef struct _SCDB_AUDIO_RM_T
 *  {
 *      UINT32         ui4_stream_id;
 *      AUD_ENC_T      e_enc;
 *      AUD_LAYER_T    e_layer;
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
 *  } SCDB_AUDIO_RM_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  e_enc                         - Audio encoding
 *  @li@c  e_layer                       - Audio layer
 *  @li@c  ui4_preroll                   - Preroll
 *  @li@c  ui4_duration                  - Duration
 *  @li@c  ui4_max_bit_rate              - Maximum bit rate
 *  @li@c  ui4_avg_bit_rate              - Average bit rate
 *  @li@c  ui4_max_packet_size           - Maximum packet size
 *  @li@c  ui4_avg_packet_size           - Average packet size
 *  @li@c  ui4_start_time                - Start time
 *  @li@c  ui2_version                   - Version
 *  @li@c  ui2_revision                  - Revision
 *  @li@c  ui2_hdr_bytes                 - Header bytes
 *  @li@c  ui2_flavor_idx                - Flavor index
 *  @li@c  ui4_granularity               - Granularity
 *  @li@c  ui4_total_bytes               - Total bytes
 *  @li@c  ui4_bytes_per_min             - Bytes per minute
 *  @li@c  ui4_bytes_per_min2            - Bytes per minute 2
 *  @li@c  ui2_intlv_factor              - Interleaving factor
 *  @li@c  ui2_intlv_blk_sz              - Interleaving block size
 *  @li@c  ui2_codec_frm_sz              - Codec frame size
 *  @li@c  ui4_user_data                 - User data
 *  @li@c  ui4_sample_rate               - Sample rate
 *  @li@c  ui4_act_sample_rate           - Actual sample rate
 *  @li@c  ui2_sample_sz                 - Sample size
 *  @li@c  ui2_num_channel               - Number of channels
 *  @li@c  ui4_intlvr_id                 - Interleaver ID
 *  @li@c  ui4_codec_id                  - Codec ID
 *  @li@c  ui1_intlv_flag                - Interleaving flag
 *  @li@c  ui1_can_copy_flag             - Can copy flag
 *  @li@c  ui1_strm_type                 - Stream type
 *  @li@c  ui1_has_intlv_ptrn            - Has interleaving PTRN
 *  @li@c  *pv_intlv_ptrn                - Interleaving PTRN data
 *  @li@c  ui4_opaque_data_sz            - Opaque data size
 *  @li@c  *pv_codec_opaque_data         - Codec opaque data
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_AUDIO_RM_T
{
    UINT32         ui4_stream_id;
    AUD_ENC_T      e_enc;    
    AUD_LAYER_T    e_layer;
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
    UINT32  ui4_codec_id;/*e_enc*/
    UINT8   ui1_intlv_flag;
    UINT8   ui1_can_copy_flag;
    UINT8   ui1_strm_type;
    UINT8   ui1_has_intlv_ptrn;
    void    *pv_intlv_ptrn;
    UINT32  ui4_opaque_data_sz;
    void    *pv_codec_opaque_data;
} SCDB_AUDIO_RM_T;

/*add for flv*/
/********* Structure for FLV audio stream ***********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for FLV audio stream
 *  @code
 *  typedef struct _SCDB_AUDIO_FLV_T
 *  {
 *      UINT32         ui4_stream_id;
 *      UINT32         ui4_scale;
 *      UINT32         ui4_rate;
 *      AUD_ENC_T      e_enc;
 *      AUD_LAYER_T    e_layer;
 *      INT16          i2_sample_size;
 *      INT16          i2_channels;
 *      INT32          i4_samples_per_sec;
 *      UINT32         ui4_duration;
 *      UINT32         ui4_start_pts;
 *      INT16          i2_block_align;
 *      BOOL           b_default;
 *  } SCDB_AUDIO_FLV_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  ui4_scale                     - Time scale
 *  @li@c  ui4_rate                      - Sample rate
 *  @li@c  e_enc                         - Audio encoding
 *  @li@c  e_layer                       - Audio layer
 *  @li@c  i2_sample_size                - Sample size
 *  @li@c  i2_channels                   - Number of channels
 *  @li@c  i4_samples_per_sec            - Samples per second
 *  @li@c  ui4_duration                  - Duration
 *  @li@c  ui4_start_pts                 - Starting PTS
 *  @li@c  i2_block_align                - Block alignment
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_AUDIO_FLV_T
{
    UINT32         ui4_stream_id;
    UINT32         ui4_scale;
    UINT32         ui4_rate;
    AUD_ENC_T      e_enc;
    AUD_LAYER_T    e_layer;
    INT16          i2_sample_size;
    INT16          i2_channels;
    INT32          i4_samples_per_sec;
    UINT32         ui4_duration;
    UINT32         ui4_start_pts;
    INT16          i2_block_align;
    BOOL           b_default;
} SCDB_AUDIO_FLV_T;

/********** Structure for FLV video stream ***********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for FLV video stream
 *  @code
 *  typedef struct         _SCDB_VIDEO_FLV_T
 *  {
 *      UINT32         ui4_stream_id;
 *      UINT32         ui4_scale;
 *      UINT32         ui4_rate;
 *      VID_ENC_T      e_enc;
 *      INT32          i4_width;
 *      INT32          i4_height;
 *      UINT32         ui4_duration;
 *      UINT32         ui4_start_pts;
 *      BOOL           b_default;
 *  } SCDB_VIDEO_FLV_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  ui4_scale                     - Time scale
 *  @li@c  ui4_rate                      - Sample rate
 *  @li@c  e_enc                         - Video encoding
 *  @li@c  i4_width                      - Video width
 *  @li@c  i4_height                     - Video height
 *  @li@c  ui4_duration                  - Duration
 *  @li@c  ui4_start_pts                 - Starting PTS
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct         _SCDB_VIDEO_FLV_T
{
    UINT32         ui4_stream_id;
    UINT32         ui4_scale;
    UINT32         ui4_rate;
    VID_ENC_T      e_enc;
    INT32          i4_width;
    INT32          i4_height;
    UINT32         ui4_duration;
    UINT32         ui4_start_pts;
    BOOL           b_default;
} SCDB_VIDEO_FLV_T;

/********* Structure for MKV subtitle stream ***********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for MKV subtitle stream
 *  @code
 *  typedef struct _SCDB_SUBTITLE_MKV_T
 *  {
 *      UINT32            ui4_stream_id;
 *      UINT32            ui4_scale;
 *      UINT32            ui4_rate;
 *      MKV_SBTL_TYPE_T   e_sbtl_type;
 *      CHAR              s_lang[ISO_639_LANG_LEN];
 *      BOOL              b_default;
 *  } SCDB_SUBTITLE_MKV_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  ui4_scale                     - Time scale
 *  @li@c  ui4_rate                      - Sample rate
 *  @li@c  e_sbtl_type                   - Subtitle type
 *  @li@c  s_lang[ISO_639_LANG_LEN]      - Language
 *  @li@c  b_default                     - Primary stream indicator
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_SUBTITLE_MKV_T
{
    UINT32            ui4_stream_id;
    UINT32            ui4_scale;
    UINT32            ui4_rate;
    MKV_SBTL_TYPE_T   e_sbtl_type;
    CHAR              s_lang[ISO_639_LANG_LEN];
    BOOL              b_default;
} SCDB_SUBTITLE_MKV_T;

/*********** Structure for MP3 audio ************/
/*------------------------------------------------------------------*/
/*! @brief  Structure for MP3 audio
 *  @code
 *  typedef struct _SCDB_AUDIO_MP3_T
 *  {
 *      AUD_ENC_T      e_enc;
 *      AUD_LAYER_T    e_layer;
 *      UINT32         ui4_bitrate;
 *      UINT32         ui4_sample_rate;
 *      UINT8          ui1_version;
 *      UINT32         ui4_frame_length;
 *  } SCDB_AUDIO_MP3_T;
 *  @endcode
 *  @li@c  e_enc                         - Audio encoding
 *  @li@c  e_layer                       - Audio layer
 *  @li@c  ui4_bitrate                   - Bitrate
 *  @li@c  ui4_sample_rate               - Sample rate
 *  @li@c  ui1_version                   - Version
 *  @li@c  ui4_frame_length              - Frame length
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_AUDIO_MP3_T
{
    AUD_ENC_T      e_enc;
    AUD_LAYER_T    e_layer;
    UINT32         ui4_bitrate;
    UINT32         ui4_sample_rate;
    UINT8          ui1_version;
    UINT32         ui4_frame_length;
} SCDB_AUDIO_MP3_T;

/********* Structure for ES video stream ***********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for ES video stream
 *  @code
 *  typedef struct _SCDB_VIDEO_ES_T
 *  {
 *      VID_ENC_T      e_enc;
 *      UINT32         ui4_stream_id;
 *  } SCDB_VIDEO_ES_T;
 *  @endcode
 *  @li@c  e_enc                         - Video encoding
 *  @li@c  ui4_stream_id                 - Stream ID
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_VIDEO_ES_T
{
    VID_ENC_T      e_enc;
    UINT32         ui4_stream_id;
} SCDB_VIDEO_ES_T;

/********* Structure for ES audio stream ***********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for ES audio stream
 *  @code
 *  typedef struct _SCDB_AUDIO_ES_T
 *  {
 *      AUD_ENC_T      e_enc;
 *      AUD_LAYER_T    e_layer;
 *      UINT32         ui4_stream_id;
 *  } SCDB_AUDIO_ES_T;
 *  @endcode
 *  @li@c  e_enc                         - Audio encoding
 *  @li@c  e_layer                       - Audio layer
 *  @li@c  ui4_stream_id                 - Stream ID
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_AUDIO_ES_T
{
    AUD_ENC_T      e_enc;
    AUD_LAYER_T    e_layer;
    UINT32         ui4_stream_id;
} SCDB_AUDIO_ES_T;
typedef struct _SCDB_SUBTITLE_PS_T
{
    UINT16           ui2_strm_id;
}SCDB_SUBTITLE_PS_T;

/********** Structure for OGG video stream ***********/
/*------------------------------------------------------------------*/
/*! @brief  Structurefor OGG video stream
 *  @code
 *  typedef struct _SCDB_VIDEO_OGG_T
 *  {
 *      UINT32         ui4_stream_id;
 *      VID_ENC_T      e_enc;
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
 *  } SCDB_VIDEO_RM_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  e_enc                         - Video encoding
 *  @li@c  ui4_preroll                   - Preroll
 *  @li@c  ui4_duration                  - Duration
 *  @li@c  ui4_max_bit_rate              - Maximum bit rate
 *  @li@c  ui4_avg_bit_rate              - Average bit rate
 *  @li@c  ui4_max_packet_size           - Maximum packet size
 *  @li@c  ui4_avg_packet_size           - Average packet size
 *  @li@c  ui4_start_time                - Start time
 *  @li@c  ui4_codec_id                  - Codec ID
 *  @li@c  ui2_frm_width                 - Frame width
 *  @li@c  ui2_frm_height                - Frame height
 *  @li@c  ui2_bit_count                 - Bit count
 *  @li@c  ui2_pad_width                 - Pad width
 *  @li@c  ui2_pad_height                - Pad height
 *  @li@c  ui4_frm_rate                  - Frame rate
 *  @li@c  ui4_spo_extra_flags           - Spo extra flags
 *  @li@c  ui4_version                   - Version
 *  @li@c  ui4_specific                  - Specific
 *  @li@c  ui1_ecc_mask                  - Ecc mask
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_VIDEO_OGG_T
{
    UINT32     ui4_stream_id;
    VID_ENC_T  e_enc;
	UINT32	   ui4_serial_num;
        UINT64	   ui8_granule_pos;	
	UINT32	   ui4_video_page_num;
	UINT32	   ui4_avg_frame_num;
	UINT32     ui4_avg_packet_size;
    UINT8	   ui1_video_id[9];			/*0x01+"video"*/
	UINT32     ui4_sub_type;
	UINT32	   ui4_size;
	UINT64	   ui8_timeunit;
	UINT64	   ui8_sample_per_unit;
	UINT32 	   ui4_default_len;
	UINT32     ui4_buffer_size;
	UINT16 	   ui2_bit_per_sample;
	UINT16	   ui2_padding;
	UINT32	   ui4_width;
	UINT32	   ui4_height;	
} SCDB_VIDEO_OGG_T;


typedef struct _SCDB_AUDIO_VORBIS_OGG_T
{
	UINT8	   ui1_vorbis_id[7];    	/*0x01+"vorbis"*/
	UINT32	   ui4_vorbis_version;
	UINT8      ui1_audio_channel;
	UINT32     ui4_audio_sample_rate;
	UINT32     ui4_bitrate_maximun;
	UINT32     ui4_bitrate_norminal;
	UINT32     ui4_bitrate_minimun;
	UINT8	   blocksize;
	UINT8      frameflag;
}SCDB_AUDIO_VORBIS_OGG_T;


typedef struct _SCDB_AUDIO_NORMAI_OGG_T
{
	UINT8	   ui1_audio_id[9];			/*0x01+"audio"*/
	UINT32     ui4_sub_type;
	UINT32	   ui4_size;
	UINT64	   ui8_timeunit;
	UINT64	   ui8_sample_per_unit;
	UINT32 	   ui4_default_len;
	UINT32     ui4_buffer_size;
	UINT16 	   ui2_bit_per_sample;
	UINT16	   ui2_padding;
	UINT16	   ui2_channels;
	UINT16	   ui2_blockalign;
	UINT32	   ui4_avg_bytes_per_sec;
}SCDB_AUDIO_NORMAL_OGG_T;

/********* Structure for OGG audio stream ***********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for OGG audio stream
 *  @code
 *  typedef struct _SCDB_AUDIO_OGG_T
 *  {
 *      UINT32         ui4_stream_id;
 *      AUD_ENC_T      e_enc;
 *      AUD_LAYER_T    e_layer;
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
 *  } SCDB_AUDIO_OGG_T;
 *  @endcode
 *  @li@c  ui4_stream_id                 - Stream ID
 *  @li@c  e_enc                         - Audio encoding
 *  @li@c  e_layer                       - Audio layer
 *  @li@c  ui4_preroll                   - Preroll
 *  @li@c  ui4_duration                  - Duration
 *  @li@c  ui4_max_bit_rate              - Maximum bit rate
 *  @li@c  ui4_avg_bit_rate              - Average bit rate
 *  @li@c  ui4_max_packet_size           - Maximum packet size
 *  @li@c  ui4_avg_packet_size           - Average packet size
 *  @li@c  ui4_start_time                - Start time
 *  @li@c  ui2_version                   - Version
 *  @li@c  ui2_revision                  - Revision
 *  @li@c  ui2_hdr_bytes                 - Header bytes
 *  @li@c  ui2_flavor_idx                - Flavor index
 *  @li@c  ui4_granularity               - Granularity
 *  @li@c  ui4_total_bytes               - Total bytes
 *  @li@c  ui4_bytes_per_min             - Bytes per minute
 *  @li@c  ui4_bytes_per_min2            - Bytes per minute 2
 *  @li@c  ui2_intlv_factor              - Interleaving factor
 *  @li@c  ui2_intlv_blk_sz              - Interleaving block size
 *  @li@c  ui2_codec_frm_sz              - Codec frame size
 *  @li@c  ui4_user_data                 - User data
 *  @li@c  ui4_sample_rate               - Sample rate
 *  @li@c  ui4_act_sample_rate           - Actual sample rate
 *  @li@c  ui2_sample_sz                 - Sample size
 *  @li@c  ui2_num_channel               - Number of channels
 *  @li@c  ui4_intlvr_id                 - Interleaver ID
 *  @li@c  ui4_codec_id                  - Codec ID
 *  @li@c  ui1_intlv_flag                - Interleaving flag
 *  @li@c  ui1_can_copy_flag             - Can copy flag
 *  @li@c  ui1_strm_type                 - Stream type
 *  @li@c  ui1_has_intlv_ptrn            - Has interleaving PTRN
 *  @li@c  *pv_intlv_ptrn                - Interleaving PTRN data
 *  @li@c  ui4_opaque_data_sz            - Opaque data size
 *  @li@c  *pv_codec_opaque_data         - Codec opaque data
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_AUDIO_OGG_T
{
    UINT32         	ui4_stream_id;
    AUD_ENC_T      	e_enc; 
	AUD_LAYER_T    	e_layer;
    UINT32			ui4_serial_num;
	UINT64			ui8_granule_pos;
	UINT32			ui4_audio_page_num;
	UINT32			ui4_avg_frame_num;
	UINT32			ui4_avg_packet_size;
	OGG_STRM_TYPE_T	t_ogg_audio_type;
    union
    {
		SCDB_AUDIO_VORBIS_OGG_T			pt_scdb_vorbis_ogg;
		SCDB_AUDIO_NORMAL_OGG_T			pt_scdb_normal_ogg;
    }u;
} SCDB_AUDIO_OGG_T;



/********* Structure for scdb extra subtitle ***********/
/*------------------------------------------------------------------*/
/*! @brief  Structure for scdb extra subtitle
 *  @code
 *  typedef struct _SCDB_EXT_SUBTITLE_T
 *  {
 *      HANDLE_T                             h_ext_subtitle;
 *      CHAR                                    c_lang_list[4];
 *      HANDLE_T                             h_related_subtitle;
 *      UINT16                                  ui2_idx;
 *      BOOL                                    b_is_related;
 *  } SCDB_AUDIO_ES_T;
 *  @endcode
 *  @li@c  h_ext_subtitle                        - main subtitle file handle
 *  @li@c  c_lang_list                             - language list
 *  @li@c  h_related_subtitle                   - related subtitle file handle
 *  @li@c  ui2_idx                                  - language id
 *  @li@c  b_is_related                           - if has related file,TRUE
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_EXT_SUBTITLE_T
{    
    HANDLE_T                            h_ext_subtitle;
    CHAR                                c_lang_list[4];
    HANDLE_T                            h_related_subtitle;
    UINT16                              ui2_idx;
    BOOL                                b_is_related;
}SCDB_EXT_SUBTITLE_T;

/* Structure for the SCDB data record */
/*------------------------------------------------------------------*/
/*! @brief  Structure for the SCDB data record
 *  @code
 *  typedef struct _SCDB_REC_T
 *  {
 *      SCDB_REC_TYPE_T             e_rec_type;
 *      UINT32                      ui4_private;
 *      union SCDB_REC_UNION_T
 *      {
 *          SCDB_AUDIO_MPEG_T       t_audio_mpeg;
 *          SCDB_VIDEO_MPEG_T       t_video_mpeg;
 *          SCDB_PRIVATE_MPEG_T     t_private_mpeg;
 *          SCDB_DSMCC_MPEG_T       t_dsmcc_mpeg;
 *          SCDB_CLOSED_CAPTION_T   t_atsc_cc;
 *          SCDB_ISDB_CAPTION_T     t_isdb_caption;
 *          SCDB_ISDB_TEXT_T        t_isdb_text;
 *          SCDB_SUBTITLE_DVB_T     t_dvb_sbtl;
 *          SCDB_TELETEXT_DVB_T     t_dvb_tltxt;
 *          SCDB_VBI_TELETEXT_DVB_T t_dvb_vbi_tltxt;
 *          SCDB_VBI_DATA_DVB_T     t_dvb_vbi_data;
 *          SCDB_AUDIO_ANALOG_T     t_audio_analog;
 *          SCDB_VIDEO_ANALOG_T     t_video_analog;
 *          SCDB_AUDIO_DIGITAL_T    t_audio_digital;
 *          SCDB_AUDIO_AVI_T        t_audio_avi;
 *          SCDB_VIDEO_AVI_T        t_video_avi;
 *          SCDB_SUBTITLE_DIVX_T    t_subtitle_avi;
 *          SCDB_AUDIO_MP4_T        t_audio_mp4;
 *          SCDB_VIDEO_MP4_T        t_video_mp4;
 *          SCDB_AUDIO_ASF_T        t_audio_asf;
 *          SCDB_VIDEO_ASF_T        t_video_asf;
 *          SCDB_AUDIO_PS_T         t_audio_ps;
 *          SCDB_VIDEO_PS_T         t_video_ps;
 *          SCDB_AUDIO_MKV_T        t_audio_mkv;
 *          SCDB_VIDEO_MKV_T        t_video_mkv;
 *          SCDB_SUBTITLE_MKV_T     t_subtitle_mkv;
 *          SCDB_AUDIO_MP3_T        t_audio_mp3;
 *          SCDB_AUDIO_ES_T         t_audio_es;
 *          SCDB_VIDEO_ES_T         t_video_es;
 *          SCDB_AUDIO_RM_T        t_audio_rm;
 *          SCDB_VIDEO_RM_T        t_video_rm;
 *      } u;
 *  } SCDB_REC_T;
 *  @endcode
 *  @li@c  e_rec_type                    - Record type
 *  @li@c  ui4_private                   - Private mask
 *  @li@c  t_audio_mpeg                  - Structure data for MPEG audio
 *  @li@c  t_video_mpeg                  - Structure data for MPEG video
 *  @li@c  t_private_mpeg                - Structure data for MPEG private data
 *  @li@c  t_dsmcc_mpeg                  - Structure data for MPEG DSMCC
 *  @li@c  t_atsc_cc                     - Structure data for ATSC Closed Caption
 *  @li@c  t_isdb_caption                - Structure data for ISDB Caption Caption
 *  @li@c  t_isdb_text                   - Structure data for ISDB Superimpose
 *  @li@c  t_dvb_sbtl                    - Structure data for DVB subtitle
 *  @li@c  t_dvb_tltxt                   - Structure data for DVB teletext
 *  @li@c  t_dvb_vbi_tltxt               - Structure data for DVB VBI teletext
 *  @li@c  t_dvb_vbi_data                - Structure data for DVB VBI data
 *  @li@c  t_audio_analog                - Structure data for analog audio
 *  @li@c  t_video_analog                - Structure data for analog video
 *  @li@c  t_audio_digital               - Structure data for digital audio
 *  @li@c  t_audio_avi                   - Structure data for AVI audio
 *  @li@c  t_video_avi                   - Structure data for AVI video
 *  @li@c  t_subtitle_avi                - Structure data for AVI subtitle
 *  @li@c  t_audio_mp4                   - Structure data for MP4 audio
 *  @li@c  t_video_mp4                   - Structure data for MP4 video
 *  @li@c  t_audio_asf                   - Structure data for ASF audio
 *  @li@c  t_video_asf                   - Structure data for ASF video
 *  @li@c  t_audio_ps                    - Structure data for Packeted Stream audio
 *  @li@c  t_video_ps                    - Structure data for Packeted Stream video
 *  @li@c  t_audio_mkv                   - Structure data for MKV audio
 *  @li@c  t_video_mkv                   - Structure data for MKV video
 *  @li@c  t_subtitle_mkv                - Structure data for MKV subtitle
 *  @li@c  t_audio_mp3                   - Structure data for MP3 audio
 *  @li@c  t_audio_es                    - Structure data for Element Stream audio
 *  @li@c  t_video_es                    - Structure data for Element Stream video
 *  @li@c  t_audio_rm                    - Structure data for RealMedia audio
 *  @li@c  t_video_rm                    - Structure data for RealMedia video
 */
/*------------------------------------------------------------------*/
typedef struct _SCDB_REC_T
{
    SCDB_REC_TYPE_T             e_rec_type;
    UINT32                      ui4_private;
    union SCDB_REC_UNION_T
    {
        SCDB_AUDIO_MPEG_T       t_audio_mpeg;
        SCDB_VIDEO_MPEG_T       t_video_mpeg;
        SCDB_PRIVATE_MPEG_T     t_private_mpeg;
        SCDB_DSMCC_MPEG_T       t_dsmcc_mpeg;
        SCDB_CLOSED_CAPTION_T   t_atsc_cc;
        SCDB_ISDB_CAPTION_T     t_isdb_caption;
        SCDB_ISDB_TEXT_T        t_isdb_text;
        SCDB_SUBTITLE_DVB_T     t_dvb_sbtl;
        SCDB_TELETEXT_DVB_T     t_dvb_tltxt;
        SCDB_VBI_TELETEXT_DVB_T t_dvb_vbi_tltxt;
        SCDB_VBI_DATA_DVB_T     t_dvb_vbi_data;
        SCDB_AUDIO_ANALOG_T     t_audio_analog;
        SCDB_VIDEO_ANALOG_T     t_video_analog;
        SCDB_AUDIO_DIGITAL_T    t_audio_digital;
        SCDB_AUDIO_AVI_T        t_audio_avi;
        SCDB_VIDEO_AVI_T        t_video_avi;
        SCDB_SUBTITLE_DIVX_T    t_subtitle_avi;
        SCDB_AUDIO_MP4_T        t_audio_mp4;
        SCDB_VIDEO_MP4_T        t_video_mp4;
        SCDB_SUBTITLE_MP4_T     t_sp_mp4;
        SCDB_AUDIO_ASF_T        t_audio_asf;
        SCDB_VIDEO_ASF_T        t_video_asf;
        SCDB_AUDIO_PS_T         t_audio_ps;
        SCDB_VIDEO_PS_T         t_video_ps;
        SCDB_SUBTITLE_PS_T      t_subtitle_ps;
        SCDB_AUDIO_MKV_T        t_audio_mkv;
        SCDB_VIDEO_MKV_T        t_video_mkv;
        SCDB_SUBTITLE_MKV_T     t_subtitle_mkv;
        SCDB_AUDIO_MP3_T        t_audio_mp3;
        SCDB_AUDIO_ES_T         t_audio_es;
        SCDB_VIDEO_ES_T         t_video_es;
        SCDB_AUDIO_RM_T        t_audio_rm;
        SCDB_VIDEO_RM_T        t_video_rm;
        SCDB_EXT_SUBTITLE_T     t_ext_subtitle;
        /*add for flv*/
        SCDB_AUDIO_FLV_T        t_audio_flv;
        SCDB_VIDEO_FLV_T        t_video_flv;
	/* add for ogg*/
	SCDB_AUDIO_OGG_T	t_audio_ogg;
	SCDB_VIDEO_OGG_T	t_video_ogg;
    } u;

} SCDB_REC_T;


/*---------------------------------------------------------------------------
 Type definition
----------------------------------------------------------------------------*/

/*
  typedef for the SCDB handle callback function.  This fucntion
  is implemented by the client of SCDB.
*/
/*------------------------------------------------------------------*/
/*! @brief  Typedef for the SCDB handle callback function.  This fucntion
 *          is implemented by the client of SCDB.
 *  @param  [in]  h_scdb            - Handle to the SCDB object.
 *  @param  [in]  e_cond            - The status flag for the SCDB object.
 *  @param  [in]  ui4_reason        - The reason for the notification. 
 *  @param  [in]  pv_tag            - Private tag for the client.
 *  @param  [in]  ui4_data          - Data pass back to the application.
 *  @see    x_scdb_open, c_scdb_open
 *  @return None
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_scdb_nfy_fct)
(
    HANDLE_T            h_scdb,
    SCDB_COND_T         e_cond,
    UINT32              ui4_reason,
    VOID*               pv_tag,
    UINT32              ui4_data
);

/*------------------------------------------------------------------*/
/*! @brief  Typedef for SCDB user defined compare function that implement
 *          customized sorting order of SCDB record.  This function is called
 *          during the sorting operation.
 *  @param  [in]  pt_scdb_rec_1     - Reference a SCDB record structure.
 *  @param  [in]  pt_scdb_rec_2     - Reference a SCDB record structure.
 *  @param  [in]  pv_tag            - Private tag info pass to the comparison
 *                                    function.
 *  @return Comparison result
 *  @retval RC_SMALLER          - `pt_scdb_rec_1' is less than `pt_scdb_rec_2'.
 *  @retval RC_EQUAL            - `pt_scdb_rec_1' is equal to  `pt_scdb_rec_2'.
 *  @retval RC_GREATER          - `pt_scdb_rec_1' is greater than `pt_scdb_rec_2'.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_scdb_sort_cmp)
(
    const SCDB_REC_T*    pt_scdb_rec_1,
    const SCDB_REC_T*    pt_scdb_rec_2,
    VOID*                pv_tag
);

/*----------------------------------------------------------------------------*/
/*! @} */
/*----------------------------------------------------------------------------*/
#endif /* _U_SCDB_H_ */


