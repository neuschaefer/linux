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
#ifndef _U_FM_DLNAFS_H_
#define _U_FM_DLNAFS_H_

#include "u_common.h"

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

typedef enum{
    DLNAR_OK = 0,
    DLNAR_DMS_CANNOT_SUPPORT = -1,
    DLNAR_INVAL = -2
}DLNAR_T;


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

#endif /* _U_FM_DLNAFS_H_ */
