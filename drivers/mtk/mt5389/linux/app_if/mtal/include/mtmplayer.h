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
 * $RCSfile: mtmplayer.h,v $
 * $Revision: #2 $
 *---------------------------------------------------------------------------*/ 
/** @file mtmplayer.h
 *  This header file declares exported APIs and data structures of Media Player Module.
 */

#ifndef MT_MPLAYER_H
#define MT_MPLAYER_H

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************
  INCLUDE FILES
********************************************************************/

#include "mttype.h"
#include "mtvdo.h"


/********************************************************************
  MACRO CONSTANT DEFINITIONS
********************************************************************/

/**Set Info Mask Type
*/
#define MTMPLAYER_MASK_NONE             0x00000000
#define MTMPLAYER_MASK_VIDEO            0x00000001
#define MTMPLAYER_MASK_AUDIO            0x00000002
#define MTMPLAYER_MASK_CAPTRACK         0x00000004
#define MTMPLAYER_MASK_SPEED            0x00000008
#define MTMPLAYER_MASK_TIMEOUT          0x00000010
#define MTMPLAYER_MASK_EXTSBTL          0x00000020
#define MTMPLAYER_MASK_NETFLIX_STREAM   0x00000040
#define MTMPLAYER_MASK_PCR              0x00000080
#define MTMPLAYER_MASK_PVR_CTRL_BLK     0x00000100
#define MTMPLAYER_MASK_PVR_REC_RANGE    0x00000200
#define MTMPLAYER_MASK_PVR_VIDEO        0x00000400
#define MTMPLAYER_MASK_PVR_AUDIO        0x00000800
#define MTMPLAYER_MASK_PVR_PCR          0x00001000
#define MTMPLAYER_MASK_LPCM_INFO        0x00002000

#define MTMPLAYER_LBA_BLOCK_SIZE        (192)  // 192 bytes
#define MTMPLAYER_TICK_PERIOD           (500)  // 500 milli-second

#define MTMPLAYER_META_SIZE             (30)

// define the rerturn value for  network app read callback function (MT_MPLAYER_GENERIC_CALLBACK_READ)
// keep the same as define in "u_net_type.h"
#define  NETWORK_FMR_OK                  ((INT32)  0)
#define  NETWORK_FMR_EOF 				 ((INT32) -13)
#define  NETWORK_FMR_ERROR               ((INT32) -99)

/********************************************************************
  MACRO FUNCTION DEFINITIONS
********************************************************************/


/********************************************************************
  TYPE DEFINITION
********************************************************************/
/**MPLAYER handle.
It is a pointer to VOID, which is used as the handle type for MTMPlayer.  
After an MTMPlayer instance has been opened, a handle is returned.  This 
handle will be used by most MTMPlayer APIs to identify the MTMPlayer instance.
*/
typedef VOID* MTMPLAYER_HANDLE;

/**Media Type.
It is used for describing the type of the media file being played back by 
MTMPlayer.  Available types include AVI, DIVX, MPEG2_PS, MPEG2_TS, ASF, 
VOB, MP4, MKV, MP3 and JPEG.  VCD and MPA types are defined but currently 
not supported.
*/
typedef enum
{
  MT_MPLAYER_MEDIA_TYPE_UNKNOWN,  // Unknown format type
  MT_MPLAYER_MEDIA_TYPE_AVI,      // AVI file
  MT_MPLAYER_MEDIA_TYPE_DIVX,     // Divx file
  MT_MPLAYER_MEDIA_TYPE_MPEG2_PS, // Mpeg2 program stream  
  MT_MPLAYER_MEDIA_TYPE_MPEG2_TS, // Mpeg2 transport stream  
  MT_MPLAYER_MEDIA_TYPE_MPEG2_ES, // Mpeg2 element stream
  MT_MPLAYER_MEDIA_TYPE_H264_ES,  // H264 element stream
  MT_MPLAYER_MEDIA_TYPE_MPEG4_ES,
  MT_MPLAYER_MEDIA_TYPE_VC1_ES,
  MT_MPLAYER_MEDIA_TYPE_ASF,      //WMV and ASF
  MT_MPLAYER_MEDIA_TYPE_VCD,      // VCD file
  MT_MPLAYER_MEDIA_TYPE_VOB,      // VOB file
  MT_MPLAYER_MEDIA_TYPE_MP4,
  MT_MPLAYER_MEDIA_TYPE_MKV,
  MT_MPLAYER_MEDIA_TYPE_MP3,      // MPEG1 Audio Layer 3 file
  MT_MPLAYER_MEDIA_TYPE_MPA,      // MPEG Audio file
  MT_MPLAYER_MEDIA_TYPE_JPEG,     // JPEG file
  MT_MPLAYER_MEDIA_TYPE_FLV,
  MT_MPLAYER_MEDIA_TYPE_RM,
  MT_MPLAYER_MEDIA_TYPE_WAVE,
  MT_MPLAYER_MEDIA_TYPE_AIF,
  MT_MPLAYER_MEDIA_TYPE_AC3,
  MT_MPLAYER_MEDIA_TYPE_AAC
} MTMPLAYER_MEDIA_TYPE_T;


/**Media Player Capability.
*/
typedef enum
{
    MT_MPLAYER_CAP_NORMAL      = 0x01, // Can be played normally   
    MT_MPLAYER_CAP_SEEK        = 0x02, // Can seek
    MT_MPLAYER_CAP_TRICKMODE   = 0x04, // Can use trick mode
    MT_MPLAYER_CAP_ROTATION    = 0x08, // Can use rotation (image only)
    MT_MPLAYER_CAP_MAX,
} MTMPLAYER_CAP_T;

/**Error Type.
It is used for describing the error type when a callback from kernel is an 
error callback which needs to pass an error code to user space.  Possible 
errors include file open error, video decoding error, audio decoding error, 
DRM (for DivX) error, image decoding error and JPEG color format error.
*/
typedef enum
{
    MT_MPLAYER_FILE_ERROR,       // Fail in opening File 
    MT_MPLAYER_VIDEO_ERROR,      // Fail in decoding video
    MT_MPLAYER_AUDIO_ERROR,      // Fail in decoding audio
    MT_MPLAYER_DRM_ERROR,        // Fail in DRM
    MT_MPLAYER_IMG_ERROR,        // Fail in decoding image
    MT_MPLAYER_JPEG_COLOR_FORMAT_ERROR, //sharp code sync
    MT_MPLAYER_DIVX_AUTH_ERROR,
    MT_MPLAYER_DIVX_RENTAL_ERROR,
    MT_MPLAYER_DIVX_ACT_ERROR,
    MT_MPLAYER_ERROR_MAX,
} MTMPLAYER_ERROR_TYPE_T;

/** for MT_MPLAYER_CALLBACK_PVR_RANGE_NOTIFY
*/
typedef enum
{
    MTMPLAYER_PVR_OUT_OF_SAFE_RANGE_START,
    MTMPLAYER_PVR_OUT_OF_SAFE_RANGE_END,
    MTMPLAYER_PVR_REACH_VALID_RANGE_START,
    MTMPLAYER_PVR_REACH_VALID_RANGE_END,
    MTMPLAYER_PVR_RANGE_MAX /* last one */
} MTMPLAYER_PVR_RANGE_COND_T;

/**Call back type.
When some event occurs during the playback of a multimedia file, a 
corresponding callback could be called to pass information to the upper 
layer.  This callback type is used for identifying the type of callback.  
Useful callback types are ERROR, FINISH and ELAPSEDTIME.  Other types are 
reserved for internal usage or non multimedia related.
*/
typedef enum
{
    MT_MPLAYER_CALLBACK_NEEDDATA,        // notification of copy buffer
    MT_MPLAYER_CALLBACK_ERROR,           // notification of play error
    MT_MPLAYER_CALLBACK_FINISH,          // notification of file end
    MT_MPLAYER_CALLBACK_CAPTION,         // notification of caption
    MT_MPLAYER_CALLBACK_BEGIN,
    MT_MPLAYER_CALLBACK_DATA_REQ_DONE,   // notification of timer
    MT_MPLAYER_CALLBACK_BUFFULL,
    MT_MPLAYER_CALLBACK_ELAPSEDTIME,     // notification of timer
    MT_MPLAYER_CALLBACK_TOTALTIME,       // notification of updated total time
    MT_MPLAYER_CALLBACK_PROFILE_CHANGE,
    MT_MPLAYER_CALLBACK_SEEK_END,        // notification of seek end
    MT_MPLAYER_CALLBACK_ABORT_DONE,      // notification of abort end
    MT_MPLAYER_CALLBACK_OPEN_TIME_OUT,   // notification of time out
    MT_MPLAYER_CALLBACK_ELAPSEDBYTE,     // notification of comsumed byte
    MT_MPLAYER_CALLBACK_DRAWSUBTITLE,
    MT_MPLAYER_CALLBACK_DRAW_EXT_SBTL,
    MT_MPLAYER_CALLBACK_PACKET_SIZE,
    MT_MPLAYER_CALLBACK_DATA_EXHAUSTED,
    MT_MPLAYER_CALLBACK_PVR_RANGE_NOTIFY,
    MT_MPLAYER_CALLBACK_MAX
} MTMPLAYER_CALLBACK_TYPE_T;

/**Generic Call back type.
This is used for registering generic file I/O operation callbacks.
*/
typedef enum
{
    MT_MPLAYER_GNRC_IO_CALLBACK_OPEN,       // callback to user to open
    MT_MPLAYER_GNRC_IO_CALLBACK_CLOSE,      // callback to user to close
    MT_MPLAYER_GNRC_IO_CALLBACK_READ,       // callback to user to get data
    MT_MPLAYER_GNRC_IO_CALLBACK_SEEK,       // callback to user to seek position
    MT_MPLAYER_GNRC_IO_CALLBACK_TELL,       // callback to user to tell current position
    MT_MPLAYER_GNRC_IO_CALLBACK_MAX,
} MTMPLAYER_GNRC_IO_CALLBACK_TYPE_T;

/**Output Color format.
*/
typedef enum
{
    MT_MPLAYER_COLOR_FORMAT_AYCBCR8888,         ///< AYCbCr display mode, 32 bit per pixel, for OSD
    MT_MPLAYER_COLOR_FORMAT_Y_CBCR422,          ///< Y/CbCr separate 422 display mode, 16 bit per pixel, for video plane
    MT_MPLAYER_COLOR_FORMAT_ARGB8888,
    MT_MPLAYER_COLOR_FORMAT_RGBA8888,            
    MT_MPLAYER_COLOR_FORMAT_RGB888,        
    MT_MPLAYER_COLOR_FORMAT_RGB565,    
    MT_MPLAYER_COLOR_FORMAT_MAX,
} MTMPLAYER_IMGCOLORFORMAT_T;

/**Output pixel format.
*/
typedef enum
{
    MT_MPLAYER_PIX_FORMAT_UYVY,         
    MT_MPLAYER_PIX_FORMAT_VYUY,         
    MT_MPLAYER_PIX_FORMAT_1555,         
    MT_MPLAYER_PIX_FORMAT_565,          
    MT_MPLAYER_PIX_FORMAT_4444,         
    MT_MPLAYER_PIX_FORMAT_8888,         
    MT_MPLAYER_PIX_FORMAT_MAX,
} MTMPLAYER_PIXFORMAT_T;

/**Rotation Option.
*/
typedef enum
{
    MT_MPLAYER_IMGROTATE_0,                     ///<no rotation
    MT_MPLAYER_IMGROTATE_90,                    ///<clockwise 90 degrees
    MT_MPLAYER_IMGROTATE_180,                   ///<clockwise 180 degrees
    MT_MPLAYER_IMGROTATE_270,                   ///<clockwise 270 degrees
    MT_MPLAYER_IMGROTATE_0_FLIP,                ///<no rotation with flip
    MT_MPLAYER_IMGROTATE_90_FLIP,               ///<clockwise 90 degrees with flip
    MT_MPLAYER_IMGROTATE_180_FLIP,              ///<clockwise 180 degrees with flip
    MT_MPLAYER_IMGROTATE_270_FLIP,              ///<clockwise 270 degrees with flip
    MT_MPLAYER_IMGROTATE_MAX
} MTMPLAYER_IMGROTATE_T;

typedef enum 
{
    MT_IMAGE_ZOOM_50_PERCENT, 
    MT_IMAGE_ZOOM_100_PERCENT,
    MT_IMAGE_ZOOM_150_PERCENT, 
    MT_IMAGE_ZOOM_200_PERCENT, 
    MT_IMAGE_ZOOM_300_PERCENT, 
    MT_IMAGE_ZOOM_400_PERCENT,
} MTMPLAYER_IMGZOOM_T;
/**Instance Option.
It is used for describing the instance type of MTMPlayer.  An MTMPlayer 
instance can be one of the following: IMAGE (for JPEG), AUDIO (for MP3), 
PLAYMGR (for Audio+Video multimedia files), AUTO (reserved) and AUDIO2 
(reserved).
*/
typedef enum
{    
    MTMPLAYER_IMAGE,
    MTMPLAYER_AUDIO,        
    MTMPLAYER_PLAYMGR,
    MTMPLAYER_AUTO,
    MTMPLAYER_AUDIO2,
    MTMPLAYER_MAX,
} MTMPLAYER_INSTANCE_T;

/**FIFO Type.
*/
typedef enum
{    
    MTMPLAYER_FIFO_V,   // Video FIFO
    MTMPLAYER_FIFO_A,   // Audio FIFO
    MTMPLAYER_FIFO_SP,  // SubPicture FIFO
    MTMPLAYER_FIFO_MAX
} MTMPLAYER_FIFOTYPE_T;

/**Mplayer FIFO parameters.
*/
typedef struct
{
    MTMPLAYER_FIFOTYPE_T eFifoType;
    UINT32 u4Sa;
    UINT32 u4Ea;
    UINT32 u4NtfySz;  // notify when data in the FIFO reaches this size
} MTMPLAYER_FIFOPARM_T;

/**LPCM Encode Type
*/
typedef enum
{
    MTMPLAYER_LPCM_TYPE_NORMAL = 0,
    MTMPLAYER_LPCM_TYPE_ADPCM,
    MTMPLAYER_LPCM_TYPE_MAX
}MTMPLAYER_LPCM_TYPE_T;

/**Tick index info.
*/
typedef struct _MTMPLAYER_TICK_INDEX_T
{
    UINT32              ui4_tick_num;  // the tick number
    UINT32              ui4_lba;       // the corresponding lba position
                                       // LBA: Logical Block Address

    // MTK driver uses tick_detail and ui4_flags.
    // please define CC_TIME_SHIFT_NO_TICK_DETAIL,
    // so MTK driver would use the same structure with MTMPLAYER_TICK_INDEX_T
    /*
    PVR_TICK_DETAIL_T*  pt_tick_detail;
    UINT32              ui4_flags; 
    */
    
} MTMPLAYER_TICK_INDEX_T;

/**Tick metadata control block.
*/
typedef struct _MTMPLAYER_TICK_CTRL_BLK_T
{
    VOID*       pv_start_address;      // the start MTMPLAYER_TICK_INDEX_T address
    VOID*       pv_end_address;        // the end MTMPLAYER_TICK_INDEX_T address
    UINT32      ui4_entry_num;         // total tick number
    UINT32      ui4_entry_size;        // sizeof MTMPLAYER_TICK_INDEX_T
    UINT32      ui4_max_vld_entry_num; // valid read tick number, suggest ui4_max_vld_entry_num = ui4_entry_num - 10
                                       // to prevent read on writing tick data, reserve some space for write.
    UINT32      ui4_tick_period;       // tick unit. suggest ui4_tick_period = 500
    UINT32      ui4_lba_init;          // the start record data offset in MTMPLAYER_LBA_BLOCK_SIZE
                                       // to prevent always start to record on same address, pick a random address
    UINT64      ui8_fifo_offset;       // fifo start offset in byte. suggest ui8_fifo_offset = 0
    UINT32      ui4_fifo_pkt_num;      // fifo size in MTMPLAYER_LBA_BLOCK_SIZE
    
} MTMPLAYER_TICK_CTRL_BLK_T;

/**Range info.
lba location: valid range start < safe range start < safe range end < valid range end
suggest: (valid range start) = (safe range start) - 4
         (safe range end) = (valid range end)  - 4
*/
typedef struct _MTMPLAYER_BUF_RANGE_INFO_T
{
    UINT32      ui4_start; /* index of range start, base is pv_start_address */
    UINT32      ui4_end;   /* index of range end, ui4_end could be smaller than ui4_start when tick index wrap over */
}   MTMPLAYER_BUF_RANGE_INFO_T;

/**Recording range info.
*/
typedef struct _MTMPLAYER_REC_BUF_RANGE_INFO_T
{
    MTMPLAYER_BUF_RANGE_INFO_T     t_valid; /* valid range for playback */
    MTMPLAYER_BUF_RANGE_INFO_T     t_safe;  /* safe range for playback, safe range should be a subset of valid range */
}   MTMPLAYER_REC_BUF_RANGE_INFO_T;


/* LPCM information for DLNA LPCM playback. */
typedef struct
{
    MTMPLAYER_LPCM_TYPE_T        eLPCMType;       // MTMPLAYER_LPCM_TYPE_NORMAL...
    BOOL                         fgBigEndian;     // 0, 1
    UINT16                       u2Channel;       // 1, 2...
    UINT16                       u2BitsPerSample; // 8, 16...
    UINT32                       u4SampleRate;    // 44100...
//    UINT64                       u8Duration;      // in second
//    UINT64                       u8FileSize;      // in bytes
} MTMPLAYER_LPCM_INFO_T;

typedef struct
{
//added by hongjun.chu 
  UINT32 u4Width;
  UINT32 u4Height;
////////////////////
  UINT32 u4SrcBuffer;
  UINT32 u4SrcWidth;
  UINT32 u4SrcHeight;
  UINT32 u4SrcHOffset;
  UINT32 u4SrcVOffset;
  UINT32 u4SrcPlaneWidth;
  UINT32 u4SrcPlaneHeight;

  UINT32 u4DstBuffer;
  UINT32 u4DstWidth;
  UINT32 u4DstHeight;
  UINT32 u4DstHOffset;
  UINT32 u4DstVOffset;
  UINT32 u4DstPlaneWidth;
  UINT32 u4DstPlaneHeight;

  UINT32 u4RotateBuffer;
  UINT32 u4RotatePlaneWidth;
  UINT32 u4RotatePlaneHeight;
  
  UINT32 u4FileSize;
  UINT32 u4SrcData;
  INT32  i4Rotate;
  UINT32 u4ColorMode;
  UINT32 u4Format;
  UINT32 u4Zoom;
//added by hongjun.chu 
  UINT32 u4PlaneWidth;
  UINT32 u4PlaneHeight;
//////
  BOOL fgHide;
  BOOL fgClean;
}IMAGE_DECDISP_INFO_T;



typedef enum
{
  IMAGE_CMD_INIT,
  IMAGE_CMD_DECODE,
  IMAGE_CMD_DISPLAY,
  IMAGE_CMD_ZOOM,
  IMAGE_CMD_MOVE,
  IMAGE_CMD_ROTATE,
  IMAGE_CMD_STOP,
  IMAGE_CMD_RELEASE,
  IMAGE_CMD_DATADONE,
  IMAGE_CMD_GETINFO
}IMAGE_CMD_TYPE;

typedef enum
{
  IMAGE_TYPE_JPEG,
  IMAGE_TYPE_WEBP,
  IMAGE_TYPE_PNG,
  IMAGE_TYPE_STILL,
  IMAGE_TYPE_MJPG,
  IMAGE_TYPE_H264,
  IMAGE_TYPE_MPEG
}IMAGE_TYPE_T;


typedef struct
{
  UINT32 u4Width;
  UINT32 u4Height;
  UINT32 u4RegionAddr;
  UINT32 u4RegionWidht;
  UINT32 u4RegionHeight;
}IMAGE_FRAME_INFO_T;



/**Transition effect for slide show.
*/
typedef enum
{
    MT_MPLAYER_EFFECT_BARWIPE_LR,
    MT_MPLAYER_EFFECT_BARWIPE_RL,
    MT_MPLAYER_EFFECT_BARWIPE_TB,
    MT_MPLAYER_EFFECT_BARWIPE_BT,
    MT_MPLAYER_EFFECT_DOORWIPE_HO,
    MT_MPLAYER_EFFECT_DOORWIPE_HC,
    MT_MPLAYER_EFFECT_DOORWIPE_VO,
    MT_MPLAYER_EFFECT_DOORWIPE_VC,
    MT_MPLAYER_EFFECT_BOXWIPE_TL,
    MT_MPLAYER_EFFECT_BOXWIPE_TR,
    MT_MPLAYER_EFFECT_BOXWIPE_BL,
    MT_MPLAYER_EFFECT_BOXWIPE_BR,
    MT_MPLAYER_EFFECT_IRISWIPE,
    MT_MPLAYER_EFFECT_NONE,    
    MT_MPLAYER_EFFECT_MAX,     
} MTMPLAYER_SLIDETYPE;

/**DRM Type.
*/
typedef enum  // this enumeration must exactly match PLAYMGR_DRMTYPE
{
    MTMPLAYER_DRMTYPE_NONE = 0,
    MTMPLAYER_DRMTYPE_PURCHASE,
    MTMPLAYER_DRMTYPE_RENTAL,
    MTMPLAYER_DRMTYPE_AUTH_ERR,
    MTMPLAYER_DRMTYPE_RENTAL_EXPIRED,
    MTMPLAYER_DRMTYPE_MAX
} MTMPLAYER_DRMTYPE;

/**Source Type.
It is used for describing the source type of the current playing multimedia 
file.  A multimedia file can be from the following sources: HIGH_SPEED_STORAGE 
(USB), NETWORK_DLNA (DLNA, reserved), NETWORK_NETFLIX (Netflix) and TIME_SHIFT 
(time-shift/PVR).
*/
typedef enum  // this enumeration must exactly match PLAYMGR_SourceType_T
{
    MTMPLAYER_SRCTYPE_UNKNOWN = 0,
    MTMPLAYER_SRCTYPE_HIGH_SPEED_STORAGE,
    MTMPLAYER_SRCTYPE_NETWORK_DLNA,
    MTMPLAYER_SRCTYPE_NETWORK_NETFLIX,
    MTMPLAYER_SRCTYPE_TIME_SHIFT,
    MTMPLAYER_SRCTYPE_NETWORK_VUDU,
    MTMPLAYER_SRCTYPE_NETWORK_FM,
    MTMPLAYER_SRCTYPE_NETWORK_DLNA_LPCM,
    MTMPLAYER_SRCTYPE_NETWORK_RHAPSODY,
    MTMPLAYER_SRCTYPE_NETWORK_XUNLEI,
    MTMPLAYER_SRCTYPE_MAX
} MTMPLAYER_SRCTYPE;

/**Netflix API type name.
*/
typedef enum
{
    NFLX_API_GET_NUM_BSTM
} MTMPLAYER_NFLX_API_TYPE;

/**Netflix API prototype: Get number of bitstream.
*/
typedef int  (*NFLX_GET_NUM_BSTM)(void);


typedef enum
{
    MT_IMAGE_JPEG,				///< image format
    MT_IMAGE_PNG,
    MT_IMAGE_WEBP,
    MT_IMAGE_MAX
} MTMPLAYER_IMG_FORMAT;

/**Mplayer setting parameters.
It is used for setting various parameters of MTMPlayer such as enabling/
disabling a specific audio track, specifying the A/V decoding format, setting 
playback speed, and so on.  Each field of this data structure is described 
below.

u4InfoMask

This is a mask value that specifies which fields in this data structure are 
valid for calling the setting API this time.  For example, if one wants to set 
audio related fields, MTMPLAYER_MASK_AUDIO should be assigned to u4InfoMask 
field and then only audio related fields such as u2ATrack, u2APid and so on 
are required to be filled before passing the whole structure to the setting 
API.  Other non-audio related fields will be ignored.  More than one mask 
values could be combined (e.g MTMPLAYER_MASK_VIDEO | MTMPLAYER_MASK_AUDIO) to 
specify multiple types of setting values at the same time.  Below are available 
masks.  Currently only VIDEO, AUDIO, SPEED and NETFLIX are valid.  Others are 
reserved for future use.

#define MTMPLAYER_MASK_NONE             0x00000000
#define MTMPLAYER_MASK_VIDEO            0x00000001
#define MTMPLAYER_MASK_AUDIO            0x00000002
#define MTMPLAYER_MASK_CAPTRACK         0x00000004
#define MTMPLAYER_MASK_SPEED            0x00000008
#define MTMPLAYER_MASK_TIMEOUT          0x00000010
#define MTMPLAYER_MASK_EXTSBTL          0x00000020
#define MTMPLAYER_MASK_NETFLIX          0x00000040
#define MTMPLAYER_MASK_PCR              0x00000080
#define MTMPLAYER_MASK_PVR_CTRL_BLK     0x00000100
#define MTMPLAYER_MASK_PVR_REC_RANGE    0x00000200
#define MTMPLAYER_MASK_PVR_VIDEO        0x00000400
#define MTMPLAYER_MASK_PVR_AUDIO        0x00000800

fgVidEn

This is a BOOLEAN value to enable/disable video.

fgAudEn

This is a BOOLEAN value to enable/disable audio.

fgCapEn

This is a BOOLEAN value to enable/disable caption(subtitle).

u2VTrack

This is an UINT16 value to specify a video track.  Currently only one video 
track is supported.  Therefore this value should always be 0.

u2ATrack

This is an UINT16 value to specify an audio track.  Valid values are 0 to 
AUDIO_TRACK_NUM - 1.

u2CapTrack

This is an UNIT16 value to specify a caption track.  Reserved for future use.

u2VPid

Reserved for future use.

u2APid

Reserved for future use.

eVdecFmt

This is an enumeration value to specify the video decoding format.  Valid 
values are as follows.

typedef enum
{
    MTVDEC_FMT_MPEG1_MPEG2,
    MTVDEC_FMT_MPEG4,
    MTVDEC_FMT_H264,
    MTVDEC_FMT_VC1,
    MTVDEC_FMT_UNKNOWN,    
    MTVDEC_FMT_MAX
}MTVDEC_FMT_T;

eAdecFmt

This is an enumeration value to specify the video decoding format.  Valid 
values are as follows.

typedef enum
{
    MTAUDDEC_FMT_UNKNOWN = 0,
    MTAUDDEC_FMT_MPEG,
    MTAUDDEC_FMT_AC3,
    MTAUDDEC_FMT_PCM,
    MTAUDDEC_FMT_MP3,
    MTAUDDEC_FMT_AAC,
    MTAUDDEC_FMT_DTS,
    MTAUDDEC_FMT_WMA,
    MTAUDDEC_FMT_RA,
    MTAUDDEC_FMT_HDCD,
    MTAUDDEC_FMT_MLP,
    MTAUDDEC_FMT_MTS,
    MTAUDDEC_FMT_A2,
    MTAUDDEC_FMT_EIAJ,
    MTAUDDEC_FMT_NICAM,
    MTAUDDEC_FMT_NUM
}MTAUDDEC_FMT_T;

i4Speed

This is an INT32 value to specify playback speed.  Valid values are as follows.

Value   Meaning
1000    1X normal playback
2000    2X fast forward
4000    4X fast forward
8000    8X fast forward
16000   16X fast forward
32000   32X fast forward
-2000   2X fast reverse
-4000   4X fast reverse
-8000   8X fast reverse
-16000  16X fast reverse
-32000  32X fast reverse
500     1/2X slow forward
250     1/4X slow forward
125     1/8X slow forward
62      1/16X slow forward

u4GetInfoTimeout

Reserved for future use.

pu1SubtitleBuffer

Reserved for future use.

eColorFormat

Reserved for JPEG usage.

i4NumOfBitstream

Reserved for Netflix usage.

rTickCtl

Reserved for time-shift/PVR usage.

rRangeInfo

Reserved for time-shift/PVR usage.

*/
typedef struct
{
    UINT32 u4InfoMask;
    
    BOOL fgVidEn;      // to enable/disable video
    BOOL fgAudEn;      // to enable/disable audio
    BOOL fgPcrEn;      // to enable/disable pcr
    BOOL fgCapEn;      // to enable/disable sub=picture
    BOOL fgExtSbtlEn;  // to enable/disable external subtitle
    
    UINT16 u2VTrack;         // Specify the video track for playback
    UINT16 u2ATrack;         // Specify the audio track for playback
    UINT16 u2CapTrack;       // Specify the caption track for playback
    UINT16 u2ExtSbtlTrack;   // Specify the external subtitle track for playback
    
    BOOL fgVidDecrypt;       // to enable/disable video decrypt
    BOOL fgAudDecrypt;       // to enable/disable audio decrypt    
    UINT8 u1VideoKeyIdx;     // sepcify the video decrypt key index
    UINT8 u1AudioKeyIdx;     // sepcify the audio decrypt key index
    UINT16 u2VPidIdx;        // Specify the video pid index for pvr playback
    UINT16 u2APidIdx;        // Specify the audio pid index for pvr playback
    UINT16 u2PcrPidIdx;      // Specify the pcr pid index for pvr playback
    UINT16 u2VPid;           // Specify the video pid for pvr playback
    UINT16 u2APid;           // Specify the audio pid for pvr playback
    UINT16 u2PcrPid;         // Specify the pcr pid for pvr playback
    MTVDEC_FMT_T    eVdecFmt;
    MTAUDDEC_FMT_T  eAdecFmt;
    
    INT32 i4Speed;                            // playback speed (+1000 = Forward 1X, +2000 = Forward 2X, -1000 = Backward 1X, ...)
    
    UINT32 u4GetInfoTimeout;                  // Get info timeout value
    UINT8* pu1SubtitleBuffer;
    INT8   i1ExtSbtlSync;                     // external subtitle SYNC timing adjustment in step, 1 step = 0.5 sec
    MTMPLAYER_IMGCOLORFORMAT_T eColorFormat;  // Desired output color format (image only)
    UINT8 u1NetflixVideoIdx;
    UINT8 u1NetflixAudioIdx;
    
    MTMPLAYER_TICK_CTRL_BLK_T      rTickCtl;
    MTMPLAYER_REC_BUF_RANGE_INFO_T rRangeInfo;
    MTMPLAYER_LPCM_INFO_T rLPCMInfo;
} MTMPLAYER_SETTING_T;

/**Mplayer status parameters.
This data structure is used for retrieving the current playback status of 
MTMPlayer.  Each field is described below.

eMediaType

This is an enumeration value to specify the current playing multimedia file's 
type.  Available types are defined in MTMPLAYER_MEDIA_TYPE_T.

eVidFormat

This is an enumeration value to specify the current playing multimedia file's 
video format.  Available formats are defined in MTVDEC_FMT_T.

eAudFormat

This is an enumeration value to specify the current playing multimedia file's 
audio format.  Available formats are defined in MTAUDDEC _FMT_T.

eCapFormat

This is an enumeration value to specify the current playing multimedia file's 
caption/subtitle format.  Reserved for future use.

u2VideoTrackNum

This is an UINT16 value to specify the number of available video tracks.

u2AudioTrackNum

This is an UINT16 value to specify the number of available audio tracks..

u2CaptionTrackNum

This is an UINT16 value to specify the number of available caption/subtitle 
tracks.

u2VideoId

This is an UINT16 value to specify the current playing video track's ID.

u2AudioId

This is an UINT16 value to specify the current playing audio track's ID.

u2CapTrackId

This is an UINT16 value to specify the current playing caption/subtitle 
track's ID.

u4TotalDuration

This is an UINT32 value to specify the total playback duration of the current 
playing multimedia file in units of millisecond.

u8FileSize

This is an UINT64 value to specify the total file size of the current playing 
multimedia file in units of BYTE.

u4PlayCap

Reserved for future use.

u4CurPos

This is an UINT32 value to specify the current playback point (time code) of 
the current playing multimedia file in units of millisecond.

u4CurTick

Reserved for time-shift/PVR usage.

u8CurBytePos

This is an UINT64 value to specify the current playback point (file position) 
of the current playing multimedia file in units of BYTE.

i4Speed

This is an INT32 value to specify playback speed.  Valid values are as follows.

Value   Meaning
1000    1X normal playback
2000    2X fast forward
4000    4X fast forward
8000    8X fast forward
16000   16X fast forward
32000   32X fast forward
-2000   2X fast reverse
-4000   4X fast reverse
-8000   8X fast reverse
-16000  16X fast reverse
-32000  32X fast reverse
500     1/2X slow forward
250     1/4X slow forward
125     1/8X slow forward
62      1/16X slow forward

i4NumOfBitstream

Reserved for Netflix usage.

fgDataExhausted

Reserved for Netflix usage.

rBufferInfo

Reserved for Netflix usage.

*/
typedef struct
{
    MTMPLAYER_MEDIA_TYPE_T   eMediaType;   // media type

    MTVDEC_FMT_T eVidFormat;               // video format
    MTAUDDEC_FMT_T  eAudFormat;            // audio format
    MTMPLAYER_CAPTION_FMT_T eCapFormat;    // caption format

    UINT16 u2VideoTrackNum;   // number of video tracks
    UINT16 u2AudioTrackNum;   // number of audio tracks
    UINT16 u2PcrTrackNum;     // number of pcr tracks
    UINT16 u2CaptionTrackNum; // number of caption tracks
    UINT16 u2ExtSbtlNum;      // number of external subtitle
    
    MTMPLAYER_DRMTYPE eDRMType;
    UINT32 u4DRMRentalLimit;
    UINT32 u4DRMRentalCount;

    // only remember one program now
    UINT16 u2VideoId;
    UINT16 u2AudioId;
    UINT16 u2CapTrackId;

    UINT32 u4TotalDuration;                // file's total time(millisecond)
    UINT64 u8FileSize;                     // total file size
    UINT32 u4PlayCap;                      // Play Capability
    UINT32 u4CurPos;                       // current position (millisecond) 
    UINT32 u4CurTick;                      // current tick, tick is 0-based time
                                           // You could combine it with the time information,
                                           // SDT / TOT / TDT in DVB and STT in ATSC
    UINT64 u8CurBytePos;                   // current position in bytes   

    INT32  i4Speed;                        // current speed
    
    INT32   i4NumOfBitstream;              // number of bitstream, for Netflix
} MTMPLAYER_STATUS_T;

typedef struct
{
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4XPos;
    UINT32 u4YPos;
    UINT32 u4StartPts;
    UINT32 u4EndPts;
    UINT32 au4ARGB[4];
    UINT32 u4WindowWidth;
    UINT32 u4WindowHeight;
}MTMPLAYER_SUBTITLE_INFO_T;

/**Mplayer callback parameters.
When some event occurs during the playback of a multimedia file, a 
corresponding callback could be called to pass information to the upper layer.  
This data structure is used for passing callback parameters.  Useful callback 
parameters are u4ElapseTime, u4TotalTime, u4ElapseByte and eErrorType.  Others 
are reserved for future use.
*/
typedef struct
{
    UCHAR* pucData;                         // caption data buffer pointer
    UINT32 u4Length;                        // caption data length
    UINT32 u4ElapseTime; 
    UINT32 u4TotalTime; 
    UINT32 u4SeekPts;
    UINT64 u4ElapseByte;
    MTMPLAYER_ERROR_TYPE_T eErrorType;      // error type 
    UINT32 u4SubtitleInfo;  // internal subtitle
    UINT32 u4ExtSbtlInfo;   // external subtitle, reserved and not used for now
    BOOL   fgDataExhausted;
    union
    {
        MTMPLAYER_PVR_RANGE_COND_T eRangeCond;  // range condition
    }u;
} MTMPLAYER_CALLBACKPARAM_T;

/**Parameters of image display.
*/
typedef struct 
{
    MTVDO_REGION_T rDstRegion;                  // Destination region
    MTMPLAYER_IMGROTATE_T eRotation;            // Rotation phase for display
    UINT32 u1PlaneId;                           // the id of the OSD/Video plane for display (image only)
    BOOL fgClean;
    BOOL fgDisplay;
} MTMPLAYER_IMGPARAM_T;

/**Parameters of image meta data.
*/
typedef struct
{
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4BPP;
    UINT32 u4ThumbnailLength;
    UINT32 u4Orientation;
    UINT32 u4APP1Length;
    UINT8 au1Maker[20];
    UINT8 au1Model[30];
    UINT8 au1Date[30];
    UINT8 *pu1Thumbnail;
    UINT8 *pu1APP0; 
    UINT8 *pu1APP1; 
    BOOL fgProgressive;
} MTMPLAYER_IMGMETA_T;

/**Parameters of A/V meta data.
*/
typedef struct
{
    UINT16 au2Title[MTMPLAYER_META_SIZE];
    UINT16 au2Album[MTMPLAYER_META_SIZE];
    UINT16 au2Artist[MTMPLAYER_META_SIZE];
    UINT16 au2Genre[MTMPLAYER_META_SIZE];
    UINT32 u4PicPos;
    UINT32 u4PicSize;
    UINT32 u4SampleFreq;
    UINT32 u4BitRate;
    UINT32 u4FrameSize; 
    UINT32 u4FrameRate; 
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4ParWidth;
    UINT32 u4ParHeight;
    UINT64 u8MediaSize;
    UINT64 u8PlayTime;
} MTMPLAYER_AVMETA_T;

/**MTMPLAYER callback function prototype.
It is the callback prototype used by MTMPlayer's callback mechanism.  The 
first argument is callback type.  The second one is a pointer to a data 
structure of callback parameters.  The last one is an additional parameter 
which is assigned when registering a specific callback function.
*/
typedef VOID (*MTMPLAYER_CALLBACK)(MTMPLAYER_CALLBACK_TYPE_T eCallbackType, 
                                     MTMPLAYER_CALLBACKPARAM_T *prParams, 
                                     UINT32 u4Param);

typedef INT32 (*MTMPLAYER_GNRC_IO_OPEN)(VOID *pvFilename, VOID **pvFd, VOID *pvExt);

typedef INT32 (*MTMPLAYER_GNRC_IO_CLOSE)(VOID *pvFd, VOID *pvExt);

typedef INT32 (*MTMPLAYER_GNRC_IO_READ)(VOID *pvFd, VOID *pvBuf, UINT32 u4Size, UINT32 u4Count, UINT32 *pu4ReadSize, VOID *pvExt);

typedef INT32 (*MTMPLAYER_GNRC_IO_TELL)(VOID *pvFd, UINT64 *pu8CurPos, VOID *pvExt);

//#  define SEEK_SET        0       /* Seek from beginning of file.  */
//#  define SEEK_CUR        1       /* Seek from current position.  */
//#  define SEEK_END        2       /* Set file pointer to EOF plus "offset" */
typedef INT32 (*MTMPLAYER_GNRC_IO_SEEK)(VOID *pvFd, UINT64 u8Offset, INT32 i4Whence, VOID *pvExt);


/********************************************************************
  EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/

/**Initialize MPlayer.  This API opens an MTMPlayer instance.
*@param phHandle        - Pointer to the handle for holding the returning 
                          instance handle.
*@param pucFileName     - File name of the multimedia file to be played back 
                          by this MTMPlayer instance.
                        - Set NULL for push mode.
                        - File instance when MTMPLAYER_SRCTYPE is 
                          MTMPLAYER_SRCTYPE_TIME_SHIFT
*@param eInputInstance  - Input instance.  For multimedia Audio+Video files, 
                          use MTMPLAYER_PLAYMGR.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_Open(MTMPLAYER_HANDLE* phHandle, CHAR* pucFileName,
                                            MTMPLAYER_INSTANCE_T eInputInstance);

/**This API closes an MTMPlayer instance.
*@param hHandle     - Instance handle.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_Close(MTMPLAYER_HANDLE hHandle);

/**This API resets an MTMPlayer instance.
*@param hHandle     - Instance handle.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_Reset(MTMPLAYER_HANDLE hHandle);

/**This API sets the A/V/SP FIFO to MPlayer.
*@param hHandle     - Instance handle.
*@param prFifo      - Pointer to the FIFO structure.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_SetFifo(MTMPLAYER_HANDLE hHandle, 
                                     MTMPLAYER_FIFOPARM_T* prFifo);

/**This API sets playback parameters.
*@param hHandle    - Instance handle.
*@param prSettings - Pointer to the data structure that holds the playback 
                     parameters to be set.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_Set(MTMPLAYER_HANDLE hHandle, 
                                     MTMPLAYER_SETTING_T* prSettings);

/**This API retrieves the current playback settings.
*@param hHandle       - Instance handle.
*@param prSettings    - Pointer to the data structure for holding the 
                        returning playback settings.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_Get(MTMPLAYER_HANDLE hHandle, 
                                     MTMPLAYER_SETTING_T* prSettings);

/**This API starts the playback.
*@param hHandle     - Instance handle.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_Start(MTMPLAYER_HANDLE hHandle);

/**This API stops the playback.
*@param hHandle     - Instance handle.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_Stop(MTMPLAYER_HANDLE hHandle);

/**This API pauses the playback.
*@param hHandle     - Instance handle.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_Pause(MTMPLAYER_HANDLE hHandle);

/**This API resumes the playback from the paused state.
*@param hHandle     - Instance handle.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_Resume(MTMPLAYER_HANDLE hHandle);

/**Step forward/backward MPlayer.
*@param hHandle      - Instance handle.
*@param i4StepAmount - Step amount (1 : step forward 1 frame a time  
                                   -1 : step backward 1 frame a time)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_Step(MTMPLAYER_HANDLE hHandle, INT32 i4StepAmount);

/**Get the buffer address and size of the input local buffer
*@param hHandle             - Instance handle.
*@param ppi1Buffer          - Avaiable buffer pointer.
*@param pu4Size             - Size of avaiable buffer.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  MTMPLAYER_GetBuffer(MTMPLAYER_HANDLE hHandle,
                            INT8** ppi1Buffer, UINT32* pu4Size);

/**This API retrieves the current playback status.
*@param hHandle   - Instance handle.
*@param prStatus  - Pointer to the data structure for holding the returning 
                    playback status.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_GetInfo(MTMPLAYER_HANDLE hHandle,
                                     MTMPLAYER_STATUS_T *prStatus);

/**This API registers a callback function.
MPlayer can uses the registered callback function to notify the upper layer. 
*@param hHandle       - Instance handle.
*@param eCallbackType - Callback type.
*@param fnCallback    - Callback function body.
*@param u4Param       - Additional parameter to be passed when calling the 
                        callback.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_RegCb(MTMPLAYER_HANDLE hHandle, 
                                     MTMPLAYER_CALLBACK_TYPE_T eCallbackType, 
                                     MTMPLAYER_CALLBACK fnCallback, 
                                     UINT32 u4Param);

/**This API registers a generic file I/O callback function.
MPlayer can uses the registered callback function to notify the upper layer. 
*@param hHandle              - Instance handle.
*@param eGenericCallbackType - Callback type.
*@param fnCallback           - Callback function body.
*@param u4Param              - Additional parameter to be passed when calling 
                               the callback.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_RegGenericCb(MTMPLAYER_HANDLE hHandle, 
                                     MTMPLAYER_GNRC_IO_CALLBACK_TYPE_T eGenericCallbackType, 
                                     VOID* fnCallback, 
                                     UINT32 u4Param);

/**This API causes the playback jumping to a specific time and continue 
playback thereby.
*@param hHandle     - Instance handle.
*@param u4SeekTime  - Time (in milliseconds) to be jumped to.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_Seek(MTMPLAYER_HANDLE hHandle, 
                                     UINT32 u4SeekTime);

/**Set the seek position by tick.
*@param hHandle     - Instance handle.
*@param u4SeekTick  - Seek tick.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_SeekTick(MTMPLAYER_HANDLE hHandle, 
                                        UINT32 u4SeekTick);

/**Set the seek position by file offset.
*@param hHandle     - Instance handle.
*@param u8SeekDest  - Seek file offset.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_SeekOfst(MTMPLAYER_HANDLE hHandle, 
                                     UINT64 u8SeekDest);

/**Set image parameters for display.
*@param hHandle      - Instance handle.
*@param prImgSetting - Image setting for display.
*@param fgPartial    - Partial flag.
*@param prRegion     - Pointer to region.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_DisplayImg(MTMPLAYER_HANDLE hHandle,
                                     MTMPLAYER_IMGPARAM_T *prImgSetting, 
                                     BOOL fgPartial, MTVDO_REGION_T *prRegion);

/**Push bitstream to kernel.
*@param hHandle                   - Instance handle.
*@param pu1Buf                    - Buffer address of bitstream data.
*@param u4Size                    - Size of the buffer.
*@param fgEof                     - Whether last data of the file/bitstream.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_PushData(MTMPLAYER_HANDLE hHandle, UINT8 *pu1Buf, 
                                                UINT32 u4Size, BOOL fgEof);


/**Get result image from kernel.
*@param hHandle                   - Instance handle.
*@param pu4Buf                    - Buffer address of jpeg output (virtual).
*@param pu4PhyBuf                 - Buffer address of jpeg output (physical).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_GetImg(MTMPLAYER_HANDLE hHandle,
                                     UINT32 *pu4Buf, UINT32 *pu4PhyBuf);

/**Get image meta data.
*@param hHandle                   - Instance handle.
*@param pMeta                     - Meta data.
*@param pu1InBuf                  - Input buf for push mode (ex: thumbnail).
*@param u4InSize                  - Input size for push mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_GetImgMetaData(MTMPLAYER_HANDLE hHandle, 
                                        MTMPLAYER_IMGMETA_T *pMeta, UINT8 *pu1InBuf, UINT32 u4InSize);

/**Get meida type.
*@param hHandle            - Instance handle.
*@param peInstance         - Pointer for holding the returning instance type.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_GetMediaType(MTMPLAYER_HANDLE hHandle, 
                                                            MTMPLAYER_INSTANCE_T *peInstance);

/**Get a/v meta data.
*@param hHandle            - Instance handle.
*@param prMeta             - Pointer for holding the returning meta data.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_GetAVMeta(MTMPLAYER_HANDLE  hHandle, MTMPLAYER_AVMETA_T *prMeta);

/**This API set the source type for MTMPlayer.
*@param eSrcType        - Source type.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTMPLAYER_SetSrcType(MTMPLAYER_SRCTYPE eSrcType);

/**Get capability.
*@param handle
*@param type
*@param parameter 1
*@param parameter 2
*@retval MTR_OK                    Supported.
*@retval MTR_NOT_OK                Not supported or failed to query.
*/
EXTERN MT_RESULT_T MTMPLAYER_GetCap(MTMPLAYER_HANDLE hHandle, MTMPLAYER_CAP_T eType, UINT32 u4Param1, UINT32 u4Param2);


EXTERN MT_RESULT_T MTMPLAYER_SwitchMode(MTMPLAYER_HANDLE  hHandle, BOOL fgPush);

EXTERN MT_RESULT_T MTMPLAYER_SetSpecialDisp(MTMPLAYER_IMGPARAM_T *prImgSetting, 
                                                      MTVDO_REGION_T *prRegion, BOOL fgPartial, 
                                                      UINT8 *pu1Buffer, UINT32 u4Width, UINT32 u4Height, 
                                                      UINT32 u4Stride, MTMPLAYER_PIXFORMAT_T eFormatType);

EXTERN MT_RESULT_T MTMPLAYER_SlideShow(MTMPLAYER_HANDLE  hHandle, MTMPLAYER_SLIDETYPE eType);

EXTERN MT_RESULT_T MTMPLAYER_PreParsingAud(MTMPLAYER_HANDLE  hHandle, 
                                                      MTMPLAYER_AVMETA_T *prMeta, UINT8 *pu1Buf);

EXTERN MT_RESULT_T MTMPLAYER_ConnectImg(BOOL fgConnect);

EXTERN MT_RESULT_T MTMPLAYER_CleanFrameBuffer(MTMPLAYER_HANDLE  hHandle);

EXTERN void MTMPLAYER_SetExtSbtlBuf(UINT8 *pu1Buf);

EXTERN BOOL MTMPLAYER_ChkExtSbtl(CHAR *pucFileName);

EXTERN BOOL MTMPLAYER_GetDRMRegCode(UINT8 *pu1Code);

EXTERN BOOL MTMPLAYER_GetDRMUIHelpInfo(UINT32 *pu4Info);

EXTERN BOOL MTMPLAYER_DRMDeactivation(UINT8 *pu1Code);

EXTERN MT_RESULT_T MTMPLAYER_SetNumOfBitstream(VOID);

EXTERN MT_RESULT_T MTMPLAYER_RegNetflixAPI(MTMPLAYER_NFLX_API_TYPE eAPI, VOID *pvAPI);

EXTERN MT_RESULT_T MTMPLAYER_SetNetflixEos(BOOL fgNetflixEos);

EXTERN MT_RESULT_T MTMPLAYER_SetEos(BOOL fgEos);
EXTERN MT_RESULT_T MTMPLAYER_ParseImageMetaData(MTMPLAYER_HANDLE hHandle,MTMPLAYER_IMGMETA_T *pMeta,
                                                     UINT32 u4Offset, UINT32 u4InSize,UINT8 *puWorkBuf);

#ifdef __cplusplus
}
#endif

#endif // MT_MPLAYER_H

