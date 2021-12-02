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
/*-----------------------------------------------------------------------------
 * $RCSfile: playmgr_if.h,v $
 *---------------------------------------------------------------------------*/
/** @file mtmplayer.h
 *  This header file declares exported APIs and data structures of Media Player Module.
 */

#ifndef PLAYMGR_IF_H
#define PLAYMGR_IF_H

/********************************************************************
  INCLUDE FILES
********************************************************************/
#include "x_typedef.h"
/********************************************************************
  MACRO CONSTANT DEFINITIONS
********************************************************************/
// It is better to sync this with SWDMX_MAX_VID_TRACK
#define PLAYMGR_MAX_VID_TRACK    0x00000008
#define PLAYMGR_MAX_AUD_TRACK    0x00000010
#define PLAYMGR_MAX_SUB_TRACK    0x00000008
#define PLAYMGR_MAX_PCR_TRACK    0x00000001
#define PLAYMGR_MAX_PROGRAM      0x00000060

#define PLAYMGR_SEEK_BGN             ((UINT8) 1)
#define PLAYMGR_SEEK_CUR             ((UINT8) 2)
#define PLAYMGR_SEEK_END             ((UINT8) 3)

#define PLAYMGR_INFO_MASK_NONE             0x00000000
#define PLAYMGR_INFO_MASK_VIDEO            0x00000001
#define PLAYMGR_INFO_MASK_AUDIO            0x00000002
#define PLAYMGR_INFO_MASK_CAPTRACK         0x00000004
#define PLAYMGR_INFO_MASK_FILESIZE         0x00000008
#define PLAYMGR_INFO_MASK_PROGRAM          0x00000010
#define PLAYMGR_INFO_MASK_PCR              0x00000020
#define PLAYMGR_INFO_MASK_NETFLIX_STREAM   0x00000040
#define PLAYMGR_INFO_MASK_PVR_CTRL_BLK     0x00000080
#define PLAYMGR_INFO_MASK_PVR_REC_RANGE    0x00000100
#define PLAYMGR_INFO_MASK_PVR_VIDEO        0x00000200
#define PLAYMGR_INFO_MASK_PVR_AUDIO        0x00000400
#define PLAYMGR_INFO_MASK_PVR_PCR          0x00000800
#define PLAYMGR_INFO_MASK_LPCM_INFO        0x00001000

#define PLAYMGR_MAX_HDR_SIZE 8

#define PLAYMGR0 (0x0)
#define PLAYMGR1 (0x1)
#define PLAYMGR2 (0x2)
#define PLAYMGR3 (0x3)
#define PLAYMGR4 (0x4)
#ifdef CC_SKYPE_FINE_INSTANCE
#define PLAYMGR_MAX_INPUT_SRC 0x5
#elif defined(CC_FBM_TWO_FBP)
#define PLAYMGR_MAX_INPUT_SRC 0x2
#else
#define PLAYMGR_MAX_INPUT_SRC 0x1
#endif
/********************************************************************
  MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
  TYPE DEFINITION
********************************************************************/
typedef VOID* PLAYMGR_HANDLE;

/**The video codec format*/
typedef enum{
    PLAYMGR_VDEC_UNKNOWN,
    PLAYMGR_VDEC_MPEG1_MPEG2,
    PLAYMGR_VDEC_DIVX311,
    PLAYMGR_VDEC_MPEG4,
    PLAYMGR_VDEC_H264,
    PLAYMGR_VDEC_MVC,
    PLAYMGR_VDEC_AVS,
    PLAYMGR_VDEC_VC1,
    PLAYMGR_VDEC_WMV7,
    PLAYMGR_VDEC_WMV8,
    PLAYMGR_VDEC_WMV9,
    PLAYMGR_VDEC_H263,
    PLAYMGR_VDEC_MJPEG,
    PLAYMGR_VDEC_RV8,
    PLAYMGR_VDEC_RV9,
    PLAYMGR_VDEC_SORENSON,
    PLAYMGR_VDEC_VP6,
    PLAYMGR_VDEC_VP8,
    PLAYMGR_VDEC_MULTITRACK,
    PLAYMGR_VDEC_MAX
}PLAYMGR_VDEC_T;


/**Audio decoder format.*/
typedef enum{
    PLAYMGR_ADEC_UNKNOWN = 0,
    PLAYMGR_ADEC_MPEG,
    PLAYMGR_ADEC_AC3,
    PLAYMGR_ADEC_PCM,
    PLAYMGR_ADEC_MP3,
    PLAYMGR_ADEC_AAC,
    PLAYMGR_ADEC_DTS,
    PLAYMGR_ADEC_WMA,
    PLAYMGR_ADEC_RA,
    PLAYMGR_ADEC_HDCD,
    PLAYMGR_ADEC_MLP,
    PLAYMGR_ADEC_MTS,
    PLAYMGR_ADEC_A2,
    PLAYMGR_ADEC_PAL,
    PLAYMGR_ADEC_FMFM,
    PLAYMGR_ADEC_NICAM,
    PLAYMGR_ADEC_TTXAAC,
    PLAYMGR_ADEC_DETECTOR,
    PLAYMGR_ADEC_MINER,
    PLAYMGR_ADEC_ADPCM
}PLAYMGR_ADEC_T;


/**Media Type
*/
typedef enum
{
  PLAYMGR_MEDIA_TYPE_UNKNOWN, // Unknown format type
  PLAYMGR_MEDIA_TYPE_AVI, // AVI
  PLAYMGR_MEDIA_TYPE_DIVX, // Divx
  PLAYMGR_MEDIA_TYPE_MPEG2_PS, // Mpeg2 program stream
  PLAYMGR_MEDIA_TYPE_MPEG2_TS, // Mpeg2 transport stream
  PLAYMGR_MEDIA_TYPE_MPEG2_TS_192, // Mpeg2 192 transport stream
  PLAYMGR_MEDIA_TYPE_MPEG2_TS_ZERO_192, // Mpeg2 4 zero byte transport stream
  PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT, // Mpeg2 time shift 192 transport stream
  PLAYMGR_MEDIA_TYPE_MPEG2_ES, // Mpeg2 element stream
  PLAYMGR_MEDIA_TYPE_H264_ES, // H264 element stream
  PLAYMGR_MEDIA_TYPE_MPEG4_ES,
  PLAYMGR_MEDIA_TYPE_AVS_ES,
  PLAYMGR_MEDIA_TYPE_VC1_ES,
  PLAYMGR_MEDIA_TYPE_VC1_WMV,
  PLAYMGR_MEDIA_TYPE_VCD,
  PLAYMGR_MEDIA_TYPE_VOB,
  PLAYMGR_MEDIA_TYPE_VRO,
  PLAYMGR_MEDIA_TYPE_MP2, // MPEG Audio Layer 2
  PLAYMGR_MEDIA_TYPE_MP3, // MPEG Audio Layer 3
  PLAYMGR_MEDIA_TYPE_MPA, // MPEG Audio
  PLAYMGR_MEDIA_TYPE_JPEG,
  PLAYMGR_MEDIA_TYPE_MP4,
  PLAYMGR_MEDIA_TYPE_MKV,
  PLAYMGR_MEDIA_TYPE_FLV,
  PLAYMGR_MEDIA_TYPE_RM,
  PLAYMGR_MEDIA_TYPE_WAVE,
  PLAYMGR_MEDIA_TYPE_AIF,
  PLAYMGR_MEDIA_TYPE_AC3,
  PLAYMGR_MEDIA_TYPE_AAC,
  PLAYMGR_MEDIA_TYPE_LPCM,
  PLAYMGR_MEDIA_TYPE_OGG,
  PLAYMGR_MEDIA_TYPE_MAX
} PLAYMGR_MEDIA_TYPE_T;


/**caption format.*/
typedef enum{
    PLAYMGR_CAPTION_UNKNOWN = 0, // Unknown format type
    PLAYMGR_CAPTION_MPEG2,
    PLAYMGR_CAPTION_AVI_TEXT,
    PLAYMGR_CAPTION_AVI_BITMAP,
    PLAYMGR_CAPTION_AVI_DXSA,
}PLAYMGR_CAPTION_FMT_T;


/**Video plane region (source/output).
*/
typedef struct
{
    UINT32 u4X;        	///< start position in x direction (u4X/1000 = start pixel/total pixel)
    UINT32 u4Y;        	///< start position in y direction (u4Y/1000 = start pixel/total pixel)
    UINT32 u4Width;  	///< width in percentage (u4Width/1000 = region width/total width)
    UINT32 u4Height; 	///< height in percentage (u4Width/1000 = region height/total width)
}PLAYMGR_VDO_REGION_T;


/**Media Player Capability
*/
typedef enum
{
    PLAYMGR_CAP_NORMAL   = 0x01, // Can be played normally
    PLAYMGR_CAP_SEEK        = 0x02, // Can seek
    PLAYMGR_CAP_TRICKMODE   = 0x04, // Can use trick mode
    PLAYMGR_CAP_ROTATION    = 0x08, // Can use rotation (image only)
    PLAYMGR_CAP_MAX         = 0x40000000
} PLAYMGR_CAP_T;

/**Error Type
*/
typedef enum
{
    PLAYMGR_FILE_ERROR,       // Fail in opening File
    PLAYMGR_VIDEO_ERROR,      // Fail in decoding video
    PLAYMGR_AUDIO_ERROR,      // Fail in decoding audio
    PLAYMGR_DRM_ERROR,        // Fail in DRM
    PLAYMGR_IMG_ERROR,        // Fail in decoding image
    PLAYMGR_ERROR_MAX
} PLAYMGR_ERROR_TYPE_T;

/**Call back type
*/
typedef enum
{
    PLAYMGR_CALLBACK_NEEDDATA,   // notification of copy buffer
    PLAYMGR_CALLBACK_ERROR,      // notification of play error
    PLAYMGR_CALLBACK_FINISH,     // notification of file end
    PLAYMGR_CALLBACK_CAPTION,    // notification of caption
    PLAYMGR_CALLBACK_FILEBEGIN,                ///< notification of fileplay begin
    PLAYMGR_CALLBACK_DATA_REQ_DONE,            ///for async reading
    PLAYMGR_CALLBACK_BUFFULL,
    PLAYMGR_CALLBACK_ELAPSEDTIME, // notification of timer
    PLAYMGR_CALLBACK_TOTALTIME,   // notification of updated total time
    PLAYMGR_CALLBACK_PROFILE_CHANGE,
    PLAYMGR_CALLBACK_SEEK_END,     // notification of seek end
    PLAYMGR_CALLBACK_ABORT_DONE,     // notification of abort end
    PLAYMGR_CALLBACK_TIME_OUT,     // notification of time out
    PLAYMGR_CALLBACK_ELAPSEDBYTE,     // notification of comsumed byte
    PLAYMGR_CALLBACK_DRAWSUBTITLE,
    PLAYMGR_CALLBACK_DRAW_EXT_SUBTITLE,
    PLAYMGR_CALLBACK_PACKET_SIZE,    //notification of netflix packet size
    PLAYMGR_CALLBACK_MAX
} PLAYMGR_CALLBACK_TYPE_T;

/**Output Color format
*/
typedef enum
{
    PLAYMGR_COLOR_FORMAT_AYCBCR8888,		    ///< AYCbCr display mode, 32 bit per pixel, for OSD
    PLAYMGR_COLOR_FORMAT_Y_CBCR422,		    ///< Y/CbCr separate 422 display mode, 16 bit per pixel, for video plane
    PLAYMGR_COLOR_FORMAT_MAX
} PLAYMGR_IMGCOLORFORMAT_T;

/**Rotation Option
*/
typedef enum
{
    PLAYMGR_IMGROTATE_0,                     ///<no rotation
    PLAYMGR_IMGROTATE_90,                    ///<clockwise 90 degrees
    PLAYMGR_IMGROTATE_180,                   ///<clockwise 180 degrees
    PLAYMGR_IMGROTATE_270,                   ///<clockwise 270 degrees
    PLAYMGR_IMGROTATE_MAX
} PLAYMGR_IMGROTATE_T;


/**FIFO Type
*/
typedef enum
{
    PLAYMGR_FIFO_V,   // Video FIFO
    PLAYMGR_FIFO_A,   // Audio FIFO
    PLAYMGR_FIFO_SP,  // SubPicture FIFO
    PLAYMGR_FIFO_MAX
} PLAYMGR_FIFOTYPE_T;


/**The condition of set audio stc*/
typedef enum{
    PLAYMGR_AUDTIME_SEEK,
    PLAYMGR_AUDTIME_PLAY,
    PLAYMGR_AUDTIME_STOP,
    PLAYMGR_AUDTIME_MAX
}PLAYMGR_AUDTIME_T;


typedef enum{
    PLAYMGR_ABREPEAT_SET_A,
    PLAYMGR_ABREPEAT_SET_B,
    PLAYMGR_ABREPEAT_CANCEL
}PLAYMGR_ABREPEAT_TYPE_T;

/**MM source type
*/
typedef enum
{
    PLAYMGR_SRC_TYPE_UNKNOWN = 0,
    PLAYMGR_SRC_TYPE_HIGH_SPEED_STORAGE,
    PLAYMGR_SRC_TYPE_NETWORK_DLNA,
    PLAYMGR_SRC_TYPE_NETWORK_NETFLIX,
    PLAYMGR_SRC_TYPE_TIME_SHIFT,
    PLAYMGR_SRC_TYPE_NETWORK_VUDU,
    PLAYMGR_SRC_TYPE_NETWORK_FM,
    PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM,
    PLAYMGR_SRC_TYPE_NETWORK_RHAPSODY,
    PLAYMGR_SRC_TYPE_NETWORK_XUNLEI,
    PLAYMGR_SRC_TYPE_MAX
}PLAYMGR_SourceType_T;


/**LPCM Encode Type
*/
typedef enum
{
    PLAYMGR_LPCM_TYPE_NORMAL = 0,
    PLAYMGR_LPCM_TYPE_ADPCM,
    PLAYMGR_LPCM_TYPE_MAX
}PLAYMGR_LPCM_TYPE_T;

/* tick metadata control block, same with PVR_TICK_CTRL_BLK_T */
typedef struct _PLAYMGR_TICK_CTRL_BLK_T
{
    VOID*       pv_start_address;
    VOID*       pv_end_address;
    UINT32      ui4_entry_num;
    UINT32      ui4_entry_size;
    UINT32      ui4_tick_period;
    UINT32      ui4_lba_init;
    UINT64      ui8_fifo_offset;
    UINT32      ui4_fifo_pkt_num;

} PLAYMGR_TICK_CTRL_BLK_T;

typedef struct _PLAYMGR_BUF_RANGE_INFO_T
{
    UINT32      ui4_start; /* index of range start */
    UINT32      ui4_end;   /* index of range end, ui4_end could be smaller than ui4_start when tick index wrap over */
}   PLAYMGR_BUF_RANGE_INFO_T;

/* SM_SESS_GNRC_SET_TYPE_REC_BUFFER_RANGE */
/* PM_SET_REC_BUF_RANGE */
/* DEMUX_SET_TYPE_REC_BUF_RANGE */
typedef struct _PLAYMGR_REC_BUF_RANGE_INFO_T
{
    PLAYMGR_BUF_RANGE_INFO_T     t_valid; /* valid range for playback */
    PLAYMGR_BUF_RANGE_INFO_T     t_safe;  /* safe range for playback, safe range should be a subset of valid range */
}   PLAYMGR_REC_BUF_RANGE_INFO_T;


/* LPCM information for DLNA LPCM playback. */
typedef struct
{
    PLAYMGR_LPCM_TYPE_T          eLPCMType;
    BOOL                         fgBigEndian;
    UINT16                       u2Channel;
    UINT16                       u2BitsPerSample;
    UINT32                       u4SampleRate;
//  UINT64                       u8Duration;
//  UINT64                       u8FileSize;
} PLAYMGR_LPCM_INFO_T;

/**Mplayer setting parameters
*/
typedef struct
{
    UINT32 u4InfoMask;

    BOOL fgVidEnable;      // to enable/disable video
    BOOL fgAudEnable;      // to enable/disable audio
    BOOL fgSubEnable;      // to enable/disable sub=picture
    BOOL fgPcrEnable;      // o enable/disable PCR

    BOOL fgVidDecrypt;      // to decrypt video
    BOOL fgAudDecrypt;      // to decrypt audio

    UINT16 u2PlayPgm;      // for the upper layer setting the program
    UINT16 u2PlayVidTrack; // for the upper layer setting the video stream
    UINT16 u2PlayAudTrack; // for the upper layer setting the audio stream
    UINT16 u2PlaySubTrack; // for the upper layer setting the sub-title stream
    UINT16 u2PlayPcrTrack; // for the upper layer setting pcr

    UINT16 u2NumPgm;         // for the upper layer getting the program
    UINT16 u2NumVideoTrack;  // for the upper layer getting the video stream id
    UINT16 u2NumAudioTrack;  // for the upper layer getting the audio stream id
    UINT16 u2NumSubTrack;    // for the upper layer getting the subtitle stream id
    UINT16 u2NumPcrTrack;    // for the upper layer getting the pcr stream id

//    INT32 i4Speed;                         // playback speed (+1000 = Forward 1X, +2000 = Forward 2X, -1000 = Backward 1X, ...)
//    UINT32 u4Pos;			               // position (millisecond)

    UINT64 u8FileSize;                     // total file size
    UINT8* pu1SubtitleBuffer;

    UINT8 u1NetflixVideoIdx;
    UINT8 u1NetflixAudioIdx;

    UINT8 u1VideoKeyIdx;
    UINT8 u1AudioKeyIdx;

    UINT16 u2VideoPidIdx;              // for set video pid in time-shift
    UINT16 u2AudioPidIdx;              // for set audio pid in time-shift
    UINT16 u2PcrPidIdx;                // for set audio pid in time-shift
    UINT16 u2VideoPid;              // for set video pid in time-shift
    UINT16 u2AudioPid;              // for set audio pid in time-shift
    UINT16 u2PcrPid;                   // for set pcr pid in time-shift
    PLAYMGR_VDEC_T eVdecFmt;
    PLAYMGR_ADEC_T eAdecFmt;
    PLAYMGR_TICK_CTRL_BLK_T rTickCtl;        //
    PLAYMGR_REC_BUF_RANGE_INFO_T rRangeInfo; //
    PLAYMGR_LPCM_INFO_T rLPCMInfo; //
} PLAYMGR_SETTING_T;

/**Mplayer status parameters
*/
typedef struct
{
    PLAYMGR_MEDIA_TYPE_T   eMediaType;   // media type

    PLAYMGR_VDEC_T aeVidFormat[PLAYMGR_MAX_VID_TRACK];  // video format
    PLAYMGR_VDEC_T aeSubVidFormat[PLAYMGR_MAX_VID_TRACK];  // video format
    PLAYMGR_ADEC_T  aeAudFormat[PLAYMGR_MAX_AUD_TRACK]; // audio format
    PLAYMGR_CAPTION_FMT_T aeCapFormat[PLAYMGR_MAX_SUB_TRACK]; // caption format

    BOOL fgGotVidEos;
    BOOL fgGotAudEos;
    BOOL fgSendEos;

    BOOL fgEnVidStrm;
    BOOL fgEnAudStrm;
    BOOL fgEnSubStrm;       // Sub title stream
    BOOL fgEnPcrStrm;       // pcr stream

    BOOL fgDisAudStrm;      // to remember if audio stream is disable by playmgr itself
    BOOL fgAudPlayed;       // to remember if audio is played by playmgr itself

    BOOL fgDisPcrStrm;      // to remember if pcr stream is disable by playmgr itself
    BOOL fgPcrPlayed;       // to remember if pcr is played by playmgr itself

    BOOL fgPlayVidDecrypt;    // for the upper layer setting the video pid index need decrypt
    BOOL fgPlayAudDecrypt;    // for the upper layer setting the audio pid index need decrypt

    UINT16 u2TotalPrmNum;    // For TS, total program may bigger then 1
    UINT16 u2VideoTracks;    // number of video tracks
    UINT16 u2AudioTracks;    // number of audio tracks
    UINT16 u2CaptionTracks;  // number of caption tracks
    UINT16 u2PcrTracks;      // number of pcr tracks

    UINT16 u2EnPgmNum;       // enable program number
    UINT16 u2EnVTrackNum;    // enable video track
    UINT16 u2EnATrackNum;    // enable audio track
    UINT16 u2EnCapTrackNum;  // enable caption track
    UINT16 u2EnPcrTrackNum;  // enable caption track

    UINT32 au4VideoId[PLAYMGR_MAX_VID_TRACK];
    UINT32 au4SubVideoId[PLAYMGR_MAX_VID_TRACK];
    UINT32 au4AudioId[PLAYMGR_MAX_AUD_TRACK]; // for the upper layer getting the audio stream id
    UINT32 au4AudioSubId[PLAYMGR_MAX_AUD_TRACK]; // for the upper layer getting the audio sub-stream id
    UINT32 au4PcrId[PLAYMGR_MAX_PCR_TRACK];      // for the upper layer getting the pcr stream id
    UINT32 au4CapTrackId[PLAYMGR_MAX_SUB_TRACK]; // for the upper layer getting the subtitle stream id

    UINT8 aau1VideoHdr[PLAYMGR_MAX_VID_TRACK][PLAYMGR_MAX_HDR_SIZE];

    UINT32 au4VideoWidth[PLAYMGR_MAX_VID_TRACK];
    UINT32 au4VideoHeight[PLAYMGR_MAX_VID_TRACK];

    UINT32 u4PlayVidIdx;    // for the upper layer setting the video pid index
    UINT32 u4PlayAudIdx;    // for the upper layer setting the audio pid index
    UINT32 u4PlayPcrIdx;    // for the upper layer setting the audio pid index

    UINT32 u4PlayVidKeyIdx;    // for the upper layer setting the video key index
    UINT32 u4PlayAudKeyIdx;    // for the upper layer setting the audio key index

    UINT32 u4PlayVidStrmId; // for the upper layer setting the video stream id
    UINT32 u4PlaySubVidStrmId; // for the upper layer setting the video stream id
    UINT32 u4PlayAudStrmId; // for the upper layer setting the audio stream id
    UINT32 u4PlayAudSubStrmId; // for the upper layer setting the audio sub-stream id
    UINT32 u4PlaySubId; // for the upper layer setting the sub-title stream id
    UINT32 u4PlayPcrStrmId; // for the upper layer setting the sub-title stream id

    UINT32 u4TotalDuration;                // file's total time(millisecond)
    UINT64 u8FileSize;                     // total file size
    UINT32 u4PlayCap;                      // Play Capability
    UINT32 u4CurPos;                       // current position (millisecond)
    UINT32 u4CurTick;                         // current speed (current tick number)
    UINT64 u8CurBytePos;                   // current position in bytes
    INT32  i4Speed;                        // current speed
    INT32  i4PreviousSpeed;                        // current speed
    BOOL   fgDataExhausted;                  //netflix data exhausted
} PLAYMGR_STATUS_T;

/**Parameters of A/V meta data
*/
typedef struct
{
	UINT16 *pu2Title;
	UINT16 *pu2Album;
	UINT16 *pu2Artist;
	UINT16 *pu2Genre;
	UINT32 u4TitleLen;
	UINT32 u4AlbumLen;
	UINT32 u4ArtistLen;
	UINT32 u4GenreLen;
	UINT32 u4SampleFreq;
	UINT32 u4BitRate;
	UINT32 u4FrameSize;
	UINT32 u4FrameRate;
	UINT32 u4ImgPos;
	UINT32 u4ImgSize;
	UINT32 u4Width;
	UINT32 u4Height;
	UINT32 u4ParWidth;
	UINT32 u4ParHeight;
	UINT64 u8MediaSize;
	UINT64 u8PlayTime;
} PLAYMGR_AVMETA_T;

/**Mplayer callback parameters
*/
typedef struct
{
    UCHAR* pucData;                         // caption data buffer pointer
    UINT32 u4Length;                        // caption data length
    PLAYMGR_ERROR_TYPE_T eErrorType;      // error type
} PLAYMGR_CALLBACKPARAM_T;

/**Parameters of image display
*/
typedef struct
{
    PLAYMGR_VDO_REGION_T rDstRegion;   		        // Destination region
    PLAYMGR_IMGROTATE_T eRotation;            // Rotation phase for display
    UINT32 u1PlaneId;
} PLAYMGR_IMGPARAM_T;

/**Mplayer FIFO parameters
*/
typedef struct
{
    PLAYMGR_FIFOTYPE_T eFifoType;
    UINT32 u4Sa;
    UINT32 u4Ea;
    UINT32 u4NtfySz;  // notify when data in the FIFO reaches this size
} PLAYMGR_FIFO_T;

/**DRM Type
*/
typedef enum  // this enumeration must exactly match MTMPLAYER_DRMTYPE
{
    PLAYMGR_DRMTYPE_NONE = 0,
    PLAYMGR_DRMTYPE_PURCHASE,
    PLAYMGR_DRMTYPE_RENTAL,
    PLAYMGR_DRMTYPE_AUTH_ERR,
    PLAYMGR_DRMTYPE_RENTAL_EXPIRED,
    PLAYMGR_DRMTYPE_MAX
} PLAYMGR_DRMTYPE;


typedef VOID (*PLAYMGR_CALLBACK)(UINT8 u1SrcId, PLAYMGR_CALLBACK_TYPE_T eCallbackType,
                                     PLAYMGR_CALLBACKPARAM_T *prParams,
                                     UINT32 u4Param);

/********************************************************************
  EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/

/**Initialize MPlayer
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Open(UINT8 u1SrcId, UCHAR* pucFileName);

/**Terminate Mplayer
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Close(UINT8 u1SrcId);

/**Initialize MPlayer
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Init(VOID);

/**Reset Mplayer
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Reset(UINT8 u1SrcId);

/**Get the stream settings to MPlayer
*@param MPlayer Handle
*@param pSettings
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Get(UINT8 u1SrcId, PLAYMGR_SETTING_T* prSettings);

/**Set the stream settings to MPlayer
*@param MPlayer Handle
*@param pSettings
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Set(UINT8 u1SrcId, PLAYMGR_SETTING_T* prSettings);

/**Get playback capability
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_GetCap(UINT8 u1SrcId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2);


/**Start Play.
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Start(UINT8 u1SrcId);


/**Stop MPlayer.
*@param handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Stop(UINT8 u1SrcId);

/**Abort.
*@param handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Abort(UINT8 u1SrcId);

/**Pause MPlayer.
*@param handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Pause(UINT8 u1SrcId);

/**Resume MPlayer.
*@param handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Resume(UINT8 u1SrcId);


BOOL PLAYMGR_ABRepeat(UINT8 u1SrcId, PLAYMGR_ABREPEAT_TYPE_T eMode);

/**Get the stream settings and status from MPlayer
*@param handle
*@param prSettings
*@param prStatus
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_GetInfo(UINT8 u1SrcId, PLAYMGR_STATUS_T *prStatus);


/**Get the audio stream settings and status from MPlayer
*@param handle
*@param prSettings
*@param prStatus
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_GetAudInfo(UINT8 u1SrcId, PLAYMGR_STATUS_T *prStatus);

/**Get the stream settings and status from MPlayer
*@param handle
*@param prStatus
*@param u4Pgm
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_GetInfoByPgm(UINT8 u1SrcId, PLAYMGR_STATUS_T *prStatus, UINT32 u4Pgm);

/**Register the callback function of MPlayer
 MPlayer can uses the registered callback function to notify the upper layer.
*@param handle
*@param eCallbackType
*@param callback funtion pointer
*@param reserved
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_RegCb(UINT8 u1SrcId, PLAYMGR_CALLBACK_TYPE_T eCallbackType,
                                     PLAYMGR_CALLBACK fnCallback,
                                     UINT32 u4Param);

/**Set the seek position by time.
*@param handle
*@param speed (1000 base -> 1x)
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Speed(UINT8 u1SrcId, INT32 i4Speed);

/**Set the seek position by time.
*@param handle
*@param seek time(millisecond)
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_SeekTime(UINT8 u1SrcId, UINT32 u4SeekTime);

/**Set the seek position by time.
*@param handle
*@param seek percent
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_SeekPercent(UINT8 u1SrcId, UINT32 u4SeekPercent);

/**Set the seek position by bytes.
*@param handle
*@param seek bytes
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_SeekPos(UINT8 u1SrcId, UINT64 u8SeekBytes);

BOOL PLAYMGR_GetMetaData(UINT8 u1SrcId, PLAYMGR_AVMETA_T *prMeta);

/**get current position(millisecond)
*@retval position
*/
UINT32 PLAYMGR_GetPos(UINT8 u1SrcId);

/**get current tick
*@retval tick number
*/
UINT32 PLAYMGR_GetTick(UINT8 u1SrcId);

/**get current position in bytes
*@retval position
*/
UINT64 PLAYMGR_GetBytePos(UINT8 u1SrcId);

BOOL PLAYMGR_SetFifo(UINT8 u1SrcId, PLAYMGR_FIFO_T *prFifo);

BOOL PLAYMGR_Step(UINT8 u1SrcId, INT32 i4StepAmount);

BOOL PLAYMGR_SetAudTime(UINT8 u1SrcId, PLAYMGR_AUDTIME_T eCmd, INT32 i4Time);

/**Nofify playmgr to get data
*@param u4Size                Received data size
*@param u8Pos                file position
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
#ifdef __KERNEL__
BOOL PLAYMGR_ReceiveData(UINT8 u1SrcId, UINT32 u4Size, UINT64 u8Pos);
#endif

BOOL PLAYMGR_ChkSubtitle(UINT8 u1SrcId, BOOL fgDrawAck);

BOOL PLAYMGR_GetDivxDRMStatus(UINT8 u1SrcId, PLAYMGR_DRMTYPE *peDRMType, UINT32 *pu4DRMRentalLimit, UINT32 *pu4DRMRentalCount);
BOOL PLAYMGR_GetDivxDRMRegCode(UINT8 u1SrcId, char *pu1Code);
BOOL PLAYMGR_GetDivxDRMUIHelpInfo(UINT8 u1SrcId, UINT32 *pu4Info);
BOOL PLAYMGR_DivxDRMDeactivation(UINT8 u1SrcId, char *pu1Code);

VOID PLAYMGR_SetSrcType(UINT8 u1SrcId, PLAYMGR_SourceType_T eSrcType);
PLAYMGR_SourceType_T PLAYMGR_GetSrcType(UINT8 u1SrcId);
VOID PLAYMGR_SetNumberOfBitstream(UINT8 u1SrcId, UINT32 u4Size);
UINT32 PLAYMGR_GetNumberOfBitstream(UINT8 u1SrcId);
BOOL PLAYMGR_SetLPCMInfo(UINT8 u1SrcId, PLAYMGR_LPCM_INFO_T rPlayLPCMInfo);
VOID PLAYMGR_FeederReset(UINT8 u1SrcId);

//#define __MODEL_slt__
#ifdef __MODEL_slt__
VOID PLAYMGR_SetContainerType(UINT32 u4ContainerType);
VOID PLAYMGR_SetSubType(UINT32 u4SubType);
void PLAYMGR_VdecFlushCtrl(void);
void PLAYMGR_VdecWaitCtrlDone(void);
#endif

#endif // MT_MPLYER_H


