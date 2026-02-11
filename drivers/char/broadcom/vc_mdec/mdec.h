/*
 * Copyright (c) 2012 Broadcom Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 *  mdec.h
 *
 *  A specification for ALL enums and structs shared all the way from the 
 *  Videocore end of media_dec all the way up through the user-level Linux lib.
 *
 *  Note: to avoid too much transcoding, the enums for the various video/audio
 *  codec configs are copied from OMX_Audio.h, so if that file updates, this
 *  one has to as well!
 */
#ifndef	_MDEC_H_
#define	_MDEC_H_

#if !defined(__KERNEL__) && !defined(__VIDEOCORE__)
  #include <stdint.h>
#endif

/******************************************************************************/
/*   IOCTL message payload types                                              */
/******************************************************************************/

/**
   Set up a Media Dec session.
**/
typedef struct {
   int audio_type;
   int video_type;

   unsigned int x;
   unsigned int y;
   unsigned int frames;
   unsigned int low_latency;

   union
   {
      MDEC_PCM_CONFIG mPCM;
   } mAudioConfig;
} MDEC_SETUP_T;

/******************************************************************************/
/**
   Send some media data to Media Dec.
**/
typedef struct {
   unsigned int data_size;
   void        *data_buf;
   int          flags;           /* see media_dec_flags_t */
   uint32_t     callback_context;
   int64_t      timestamp;
} MDEC_SEND_DATA_T;

/******************************************************************************/
/**
   Set the volume, in milliBels (0.01dB)
**/
typedef struct {
   int volume_in_mB;
} MDEC_SET_VOLUME_T;

/******************************************************************************/
/**
   Set the clock
**/
typedef struct {
   int rate;
} MDEC_SET_CLOCK_T;

/******************************************************************************/
/**
   Set the source region coordinates.
**/
typedef struct {
   int x;
   int y;
   int width;
   int height;
} MDEC_SET_SOURCE_REGION_T;

/******************************************************************************/
/**
   Set the destination region parameters
**/
typedef struct {
   int display;
   int fullscreen;
   int layer;
   int x;
   int y;
   int width;
   int height;
   int pixel_aspect_x;
   int pixel_aspect_y;
   int transform;       /**< see media_dec_transform_t */
   int mode;            /**< see media_dec_aspect_flags_t */
} MDEC_SET_DEST_REGION_T;

/******************************************************************************/
/**
   Mute ore unmute the audio.
**/
typedef struct {
   int muted;
} MDEC_SET_MUTED_T;

/******************************************************************************/
/**
   Set the video transparency alpha.
**/
typedef struct {
   int alpha;
} MDEC_SET_TRANSPARENCY_T;

/******************************************************************************/
/**
   Get a frame from the video stream.
**/
typedef struct {
   unsigned int width;
   unsigned int height;
   unsigned int stride;
   void        *buffer;
   unsigned int bytes;
} MDEC_GET_FRAME_T;

/******************************************************************************/
/**
   Wait for a callback from the Media Dec server.
**/
typedef struct {
   uint32_t    mReason;  /* see mdec_callback_reason_t */
   uint64_t    mData;
   uint32_t    mContext;
} MDEC_WAIT_FOR_CALLBACK_T;

/******************************************************************************/
/**
   Query a property of the Media Dec server.
**/

typedef enum {
   MDEC_PROPERTY_VOLUME = 0x1,
   MDEC_PROPERTY_VIDEO_BUFFER_LEVEL,
   MDEC_PROPERTY_AUDIO_BUFFER_LEVEL,
   MDEC_PROPERTY_RESOLUTION,
} mdec_property_t;

typedef struct {
   int      property_id;      /* from mdec_property_t */
   union {
      int          volume;
      unsigned int level;
      struct {
         uint16_t width;
         uint16_t height;
      } resolution;
   } out_value;
} MDEC_GET_PROPERTY_T;

/******************************************************************************/
/**
   fade volume, duration and type.
**/
typedef struct {
   /* if init_volume is -1 the fader should
    * start from its current volume. */
   int init_volume;
   int target_volume;
   unsigned duration;
   unsigned type;
} MDEC_SET_FADE_T;

typedef struct {
   int volume;
} MDEC_GET_FADE_T;

typedef struct {
   int offset;
} MDEC_SYNC_OFFSET_T;

/******************************************************************************/
/**
   IOCTL Definitions
**/

#define MAX_MDEC_IOCTL_CMD_SIZE (256)

typedef enum {
   MDEC_IOCTL_PLAYER_SETUP_ID = 0x2,
   MDEC_IOCTL_PLAYER_SEND_VIDEO_DATA_ID,
   MDEC_IOCTL_PLAYER_SEND_AUDIO_DATA_ID,
   MDEC_IOCTL_PLAYER_STOP_ID,
   MDEC_IOCTL_PLAYER_PAUSE_ID,
   MDEC_IOCTL_PLAYER_RESUME_ID,
   MDEC_IOCTL_PLAYER_SET_VOLUME_ID,
   MDEC_IOCTL_PLAYER_GET_PROPERTY_ID,
   MDEC_IOCTL_PLAYER_SET_SOURCE_REGION_ID,
   MDEC_IOCTL_PLAYER_SET_DEST_REGION_ID,
   MDEC_IOCTL_PLAYER_SET_MUTED_ID,
   MDEC_IOCTL_PLAYER_SET_TRANSPARENCY_ID,
   MDEC_IOCTL_PLAYER_GET_FRAME_ID,
   MDEC_IOCTL_PLAYER_WAIT_FOR_CALLBACK_ID,
   MDEC_IOCTL_PLAYER_STOP_CALLBACK_DISPATCH_ID,
   MDEC_IOCTL_PLAYER_SET_FADE_ID,
   MDEC_IOCTL_PLAYER_GET_FADE_ID,
   MDEC_IOCTL_PLAYER_SET_CLOCK_ID,
   MDEC_IOCTL_PLAYER_STEP_ID,
   MDEC_IOCTL_PLAYER_CLEAR_ID,
   MDEC_IOCTL_PLAYER_SET_SYNC_OFFSET_ID,
} mdec_ioctl_player_t;

#define MDEC_IOCTL_PLAYER_SETUP                   _IOW(  'S', MDEC_IOCTL_PLAYER_SETUP_ID, MDEC_SETUP_T )
#define MDEC_IOCTL_PLAYER_SEND_AUDIO_DATA         _IOW(  'S', MDEC_IOCTL_PLAYER_SEND_AUDIO_DATA_ID, MDEC_SEND_DATA_T )
#define MDEC_IOCTL_PLAYER_SEND_VIDEO_DATA         _IOW(  'S', MDEC_IOCTL_PLAYER_SEND_VIDEO_DATA_ID, MDEC_SEND_DATA_T )
#define MDEC_IOCTL_PLAYER_STOP                    _IO(   'S', MDEC_IOCTL_PLAYER_STOP_ID )
#define MDEC_IOCTL_PLAYER_PAUSE                   _IO(   'S', MDEC_IOCTL_PLAYER_PAUSE_ID )
#define MDEC_IOCTL_PLAYER_RESUME                  _IO(   'S', MDEC_IOCTL_PLAYER_RESUME_ID )
#define MDEC_IOCTL_PLAYER_SET_VOLUME              _IOW(  'S', MDEC_IOCTL_PLAYER_SET_VOLUME_ID, MDEC_SET_VOLUME_T )
#define MDEC_IOCTL_PLAYER_SET_CLOCK               _IOW(  'S', MDEC_IOCTL_PLAYER_SET_CLOCK_ID, MDEC_SET_CLOCK_T )
#define MDEC_IOCTL_PLAYER_STEP                    _IO(   'S', MDEC_IOCTL_PLAYER_STEP_ID )
#define MDEC_IOCTL_PLAYER_CLEAR                   _IO(   'S', MDEC_IOCTL_PLAYER_CLEAR_ID )
#define MDEC_IOCTL_PLAYER_GET_PROPERTY            _IOWR( 'S', MDEC_IOCTL_PLAYER_GET_PROPERTY_ID, MDEC_GET_PROPERTY_T )
#define MDEC_IOCTL_PLAYER_SET_SOURCE_REGION       _IOW(  'S', MDEC_IOCTL_PLAYER_SET_SOURCE_REGION_ID, MDEC_SET_SOURCE_REGION_T )
#define MDEC_IOCTL_PLAYER_SET_DEST_REGION         _IOW(  'S', MDEC_IOCTL_PLAYER_SET_DEST_REGION_ID, MDEC_SET_DEST_REGION_T )
#define MDEC_IOCTL_PLAYER_SET_MUTED               _IOW(  'S', MDEC_IOCTL_PLAYER_SET_MUTED_ID, MDEC_SET_MUTED_T )
#define MDEC_IOCTL_PLAYER_SET_TRANSPARENCY        _IOW(  'S', MDEC_IOCTL_PLAYER_SET_TRANSPARENCY_ID, MDEC_SET_TRANSPARENCY_T )
#define MDEC_IOCTL_PLAYER_GET_FRAME               _IOW(  'S', MDEC_IOCTL_PLAYER_GET_FRAME_ID, MDEC_GET_FRAME_T )
#define MDEC_IOCTL_PLAYER_WAIT_FOR_CALLBACK       _IOR(  'S', MDEC_IOCTL_PLAYER_WAIT_FOR_CALLBACK_ID, MDEC_WAIT_FOR_CALLBACK_T )
#define MDEC_IOCTL_PLAYER_STOP_CALLBACK_DISPATCH  _IO(   'S', MDEC_IOCTL_PLAYER_STOP_CALLBACK_DISPATCH_ID )
#define MDEC_IOCTL_PLAYER_SET_FADE                _IOW(  'S', MDEC_IOCTL_PLAYER_SET_FADE_ID, MDEC_SET_FADE_T )
#define MDEC_IOCTL_PLAYER_GET_FADE                _IOWR( 'S', MDEC_IOCTL_PLAYER_GET_FADE_ID, MDEC_GET_FADE_T )
#define MDEC_IOCTL_PLAYER_SET_SYNC_OFFSET         _IOW(  'S', MDEC_IOCTL_PLAYER_SET_SYNC_OFFSET_ID, MDEC_SYNC_OFFSET_T )

/******************************************************************************/
#endif /* _MDEC_H_ */
/******************************************************************************/
