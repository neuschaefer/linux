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
/*! @file u_fm_dlnafs.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *             This file contains public API for manipulating dlna devices.
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_FM_DLNAFS_H_
#define _U_FM_DLNAFS_H_

#include "u_common.h"



/*!
 * @defgroup    groupMW_FM_DLNAFS DLNA File System
 * @ingroup     groupMW_FM
 * @brief    Provide extentional API about DLNA FS.
 * %dlna fs normally needn't access directly but from FM API. But dlna has some special
 * functions that can't be used from FM API, those functions is supplied specially here.
 *  
 * @see  groupMW_FM
 *
 * @{
 * */

/*! @enum DLNA_FILE_CONTENT_TYPE_T
 *  @brief dlna content type for filtering.
 *  @code
 *  typedef enum
 *  {
 *      DLNA_FILE_CONTENT_TYPE_CARELESS = -1,
 *      DLNA_FILE_CONTENT_TYPE_EXT_NAME = 0,
 *      DLNA_FILE_CONTENT_TYPE_DIR,
 *      DLNA_FILE_CONTENT_TYPE_FILE,
 *      DLNA_FILE_CONTENT_TYPE_IMAGE,
 *      DLNA_FILE_CONTENT_TYPE_IMAGE_PHOTO,
 *      DLNA_FILE_CONTENT_TYPE_AUDIO,
 *      DLNA_FILE_CONTENT_TYPE_AUDIO_MUSICTRACK,
 *      DLNA_FILE_CONTENT_TYPE_AUDIO_BROADCAST,
 *      DLNA_FILE_CONTENT_TYPE_AUDIO_BOOK,
 *      DLNA_FILE_CONTENT_TYPE_VIDEO,
 *      DLNA_FILE_CONTENT_TYPE_VIDEO_MOVIE,
 *      DLNA_FILE_CONTENT_TYPE_VIDEO_BROADCAST,
 *      DLNA_FILE_CONTENT_TYPE_VIDEO_MUSIC_VIDEOCLIP,
 *      DLNA_FILE_CONTENT_TYPE_PLAYLIST,
 *      DLNA_FILE_CONTENT_TYPE_TEXT,
 *      DLNA_FILE_CONTENT_TYPE_END
 *  } DLNA_FILE_CONTENT_TYPE_T;
 *  @endcode
 *  @li@c DLNA_FILE_CONTENT_TYPE_CARELESS      -  Don't set any filter specification.
 *  @li@c DLNA_FILE_CONTENT_TYPE_EXT_NAME      -  For filter extention name part which contains a specific sub-string.
 *  @li@c DLNA_FILE_CONTENT_TYPE_DIR           -  Filter dir.
 *  @li@c DLNA_FILE_CONTENT_TYPE_FILE          -  Filter file.
 *  @li@c DLNA_FILE_CONTENT_TYPE_IMAGE         -  Filter image.
 *  @li@c DLNA_FILE_CONTENT_TYPE_IMAGE_PHOTO         -  Filter image photo.
 *  @li@c DLNA_FILE_CONTENT_TYPE_AUDIO         -  Filter audio.
 *  @li@c DLNA_FILE_CONTENT_TYPE_AUDIO_BOOK    -  Filter audio book.
 *  @li@c DLNA_FILE_CONTENT_TYPE_AUDIO_MUSICTRACK    -  Filter audio musicTrack.
 *  @li@c DLNA_FILE_CONTENT_TYPE_AUDIO_BROADCAST    -  Filter audio broadcast.
 *  @li@c DLNA_FILE_CONTENT_TYPE_VIDEO         -  Filter video.
 */ 
typedef enum{
    DLNA_FILE_CONTENT_TYPE_CARELESS = -1,
    DLNA_FILE_CONTENT_TYPE_EXT_NAME = 0,
    DLNA_FILE_CONTENT_TYPE_DIR,
    DLNA_FILE_CONTENT_TYPE_FILE,
    DLNA_FILE_CONTENT_TYPE_IMAGE,
    DLNA_FILE_CONTENT_TYPE_IMAGE_PHOTO,
    DLNA_FILE_CONTENT_TYPE_AUDIO,
    DLNA_FILE_CONTENT_TYPE_AUDIO_MUSICTRACK,
    DLNA_FILE_CONTENT_TYPE_AUDIO_BROADCAST,
    DLNA_FILE_CONTENT_TYPE_AUDIO_BOOK,
    DLNA_FILE_CONTENT_TYPE_VIDEO,
    DLNA_FILE_CONTENT_TYPE_VIDEO_MOVIE,
    DLNA_FILE_CONTENT_TYPE_VIDEO_BROADCAST,
    DLNA_FILE_CONTENT_TYPE_VIDEO_MUSIC_VIDEOCLIP,
    DLNA_FILE_CONTENT_TYPE_PLAYLIST,
    DLNA_FILE_CONTENT_TYPE_TEXT,
    DLNA_FILE_CONTENT_TYPE_END
} DLNA_FILE_CONTENT_TYPE_T;

/*----------------------------------------------------------------------------------------------*/
/*! @enum DLNAR_T
 *  @brief Return code of dlnafs.
 *  @code
 *  typedef enum{
 *      DLNAR_OK = 0,
 *      DLNAR_DMS_CANNOT_SUPPORT = -1,
 *      DLNAR_INVAL = -2
 *  }DLNAR_T;
 *  @endcode
 *
 *  @li@c DLNAR_OK    - OK.
 *  @li@c DLNAR_DMS_CANNOT_SUPPORT    - DMS can't support.
 *  @li@c DLNAR_INVAL - Invalid arguments.
 */
typedef enum{
    DLNAR_OK = 0,
    DLNAR_DMS_CANNOT_SUPPORT = -1,
    DLNAR_INVAL = -2
}DLNAR_T;

/*----------------------------------------------------------------------------------------------*/
/*! @enum DLNA_ORDERBY_TYPE_T
 *  @brief Specify the orderby type.
 *  @code
 *  typedef enum{
 *      DLNA_ORDER_UNDEF = -1,      
 *      DLNA_ORDER_TITLE_ASCEND = 0,
 *      DLNA_ORDER_TITLE_DESCEND,   
 *      DLNA_ORDER_DATE_ASCEND,
 *      DLNA_ORDER_DATE_DESCEND,    
 *      DLNA_ORDER_RATE_ASCEND,
 *      DLNA_ORDER_RATE_DESCEND,    
 *      DLNA_ORDER_ARTIST_ASCEND,
 *      DLNA_ORDER_ARTIST_DESCEND,  
 *      DLNA_ORDER_GENRE_ASCEND,
 *      DLNA_ORDER_GENRE_DESCEND,   
 *      DLNA_ORDER_ALBUM_ASCEND,
 *      DLNA_ORDER_ALBUM_DESCEND,   
 *      DLNA_ORDER_SIZE_ASCEND,
 *      DLNA_ORDER_SIZE_DESCEND,    
 *  }DLNA_ORDERBY_TYPE_T;
 *  @endcode
 *  
 *  @li@c  DLNA_ORDER_UNDEF     - undefine.
 *  @li@c  DLNA_ORDER_TITLE_ASCEND   - In ascending order of title. 
 *  @li@c  DLNA_ORDER_TITLE_DESCEND  - In descending order of title. 
 *  @li@c  DLNA_ORDER_DATE_ASCEND    - Sort by date ascending.
 *  @li@c  DLNA_ORDER_DATE_DESCEND   - Sort by date descending.
 *  @li@c  DLNA_ORDER_RATE_ASCEND    - Sort by rate ascending.
 *  @li@c  DLNA_ORDER_RATE_DESCEND   - Sort by rate descending.
 *  @li@c  DLNA_ORDER_ARTIST_ASCEND  - Sort by artist ascending.
 *  @li@c  DLNA_ORDER_ARTIST_DESCEND - Sort by artist descending.
 *  @li@c  DLNA_ORDER_GENRE_ASCEND   - Sort by genri ascending.
 *  @li@c  DLNA_ORDER_GENRE_DESCEND  - Sort by genri descending.
 *  @li@c  DLNA_ORDER_ALBUM_ASCEND   - Sort by album ascending.
 *  @li@c  DLNA_ORDER_ALBUM_DESCEND  - Sort by album descending.
 *  @li@c  DLNA_ORDER_SIZE_ASCEND    - Sort by size ascending.
 *  @li@c  DLNA_ORDER_SIZE_DESCEND   - Sort by size descending.
 */
typedef enum{
    DLNA_ORDER_UNDEF = -1,      /* undefine. */
    DLNA_ORDER_TITLE_ASCEND = 0,         /* In ascending order of title. */
    DLNA_ORDER_TITLE_DESCEND,        /* In descending order of title. */
    DLNA_ORDER_DATE_ASCEND,
    DLNA_ORDER_DATE_DESCEND,         /* sort by date. */
    DLNA_ORDER_RATE_ASCEND,
    DLNA_ORDER_RATE_DESCEND,         /* sort by rate. */
    DLNA_ORDER_ARTIST_ASCEND,
    DLNA_ORDER_ARTIST_DESCEND,       /* sort by artist */
    DLNA_ORDER_GENRE_ASCEND,
    DLNA_ORDER_GENRE_DESCEND,        /* sort by genre */
    DLNA_ORDER_ALBUM_ASCEND,
    DLNA_ORDER_ALBUM_DESCEND,        /* sort by album */
    DLNA_ORDER_SIZE_ASCEND,
    DLNA_ORDER_SIZE_DESCEND,         /* sort by file size. */
}DLNA_ORDERBY_TYPE_T;


/*----------------------------------------------------------------------------------------------*/
/*! @struct DLNA_MEDIA_INFO_T
 *  @brief This struct supplies the fields used by LPCM playing.
 *  @code
 *  typedef struct _DLNA_MEDIA_INFO_T
 *  {
 *      UINT64                      ui8_size;
 *      UINT32                      ui4_sampling_frequency;
 *      UINT16                      ui2_bits_per_sample;
 *      UINT16                      ui2_channels;
 *      UINT32                      ui4_bitrate;
 *      UINT32                      ui4_resolution;
 *      UINT16                      ui4_colorDepth;         
 *  } DLNA_MEDIA_INFO_T;
 *  @endcode
 *  @li@c      ui8_size;               -  File size
 *  @li@c      ui4_sampling_frequency; -  sampling frequency
 *  @li@c      ui2_bits_per_sample;    -  bits per sample
 *  @li@c      ui2_channels;           -  channels number
 *  @li@c      ui4_bitrate;            -  bit rate
 *  @li@c      ui4_resolution;         -  resolution of the content
 *  @li@c      ui4_colorDepth;         -  color depth of the content
*/
typedef struct _DLNA_MEDIA_INFO_T
{
    
    /* common */
    UINT64                      ui8_size;
    /* size */

    /* audio */
    UINT32                      ui4_sampling_frequency;
    /* sampleFrequency */
    UINT16                      ui2_bits_per_sample;
    
    UINT16                      ui2_channels;
    /* nrAudioChannels */
    UINT32                      ui4_bitrate;
    /* bitrate */

    /* image */
    UINT32                      ui4_resolution;
    /* resolution */
    UINT16                      ui4_colorDepth;
    /* colorDepth */
} DLNA_MEDIA_INFO_T;

/*! @}  */
#endif /* _U_FM_DLNAFS_H_ */
