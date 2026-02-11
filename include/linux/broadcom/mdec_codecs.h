/*
 *  mdec_codecs.h
 *
 *  A specification for ALL enums and structs shared all the way from the Videocore end of media_dec
 *  all the way up through the user-level Linux lib.
 *
 *  Note: to avoid too much transcoding, the enums for the various video/audio codec configs are
 *  copied from OMX_Audio.h, so if that file updates, this one has to as well!
 *
 *  TODO: Rename this file since it has more than just codec enums now :)
 */

#ifndef _mdec_codecs_h
#define _mdec_codecs_h

#if !defined(__KERNEL__) && !defined(__VIDEOCORE__)
  #include <stdint.h>
#endif

/** User-level callback enumerations  *************************************************************/

typedef enum
{
   MDEC_CALLBACK_REASON_AUDIO_OWNERSHIP_RETURN, /**< Audio buffer ownership returned */
   MDEC_CALLBACK_REASON_VIDEO_OWNERSHIP_RETURN, /**< Video buffer ownership returned */
   MDEC_CALLBACK_REASON_AUDIO_TIME,             /**< Time. Data is the 64-bit tim (in us) when a marked audio frame was rendered */
   MDEC_CALLBACK_REASON_VIDEO_TIME,             /**< Time. Data is the 64-bit tim (in us) when a marked video frame was rendered */
   MDEC_CALLBACK_REASON_AUDIO_UNDERRUN,         /**< A buffer underrun occurred during audio playback */
   MDEC_CALLBACK_REASON_VIDEO_UNDERRUN,         /**< A buffer underrun occurred during video playback */
   MDEC_CALLBACK_REASON_AUDIO_END_OF_STREAM,    /**< End of audio stream reached */
   MDEC_CALLBACK_REASON_VIDEO_END_OF_STREAM,    /**< End of video stream reached */
   MDEC_CALLBACK_REASON_USER_STOPPED,           /**< Client stopped callback with MDEC_IOCTL_PLAYER_STOP_CALLBACK_DISPATCH */
   MDEC_CALLBACK_REASON_VIDEO_FRAME_SYNC,       /**< video decoder dropped or repeated a video frame */
} MDEC_CALLBACK_REASON;

/** Codec type enumerations  *********************************************************************/

typedef enum
{
   MEDIA_DEC_VIDEO_CodingUnused,     /**< Value when coding is N/A */
   MEDIA_DEC_VIDEO_CodingMPEG4,      /**< MPEG-4 */
   MEDIA_DEC_VIDEO_CodingAVC,        /**< H.264/AVC */
   MEDIA_DEC_VIDEO_CodingWMV,        /**< Windows Media Video */
   MEDIA_DEC_VIDEO_CodingVC1,        /**< Windows Media Video */
   MEDIA_DEC_VIDEO_CodingMPEG2,      /**< MPEG-2 */
} MEDIA_DEC_VIDEO_T;

typedef enum
{
   MEDIA_DEC_AUDIO_CodingUnused = 0,  /**< Placeholder value when coding is N/A  */
   MEDIA_DEC_AUDIO_CodingPCM,         /**< Any variant of PCM coding */
   MEDIA_DEC_AUDIO_CodingAAC,
   MEDIA_DEC_AUDIO_CodingAAC_Unframed,
   MEDIA_DEC_AUDIO_CodingMP3,         /**< Any variant of MP3 encoded data */
   MEDIA_DEC_AUDIO_CodingDD,          /**< Any variant of Dolby Digital */
   MEDIA_DEC_AUDIO_CodingDDP,         /**< Any variant of Dolby Digital Plus */
   MEDIA_DEC_AUDIO_CodingDTS,         /**< Any variant of DTS */
   MEDIA_DEC_AUDIO_CodingWMA,         /**< Any variant of WMA encoded data */
   MEDIA_DEC_AUDIO_CodingWMAPRO,         /**< Any variant of WMA encoded data */
} MEDIA_DEC_AUDIO_T;

typedef enum
{
   MEDIA_DEC_PCM_FLAG_SIGNED      = 0x01, /* Indicates PCM data as signed (versus unsigned) */
   MEDIA_DEC_PCM_FLAG_BIG_ENDIAN  = 0x02, /* Indicates PCM data as big endian (vs little endian) */
   MEDIA_DEC_PCM_FLAG_INTERLEAVED = 0x04, /* Indicates PCM data as interleaved (vs block/non-interleaved) */
} MDEC_PCM_FLAGS;

/** PCM mode type  ******************************************************************************/

typedef struct MDEC_PCM_CONFIG
{
   uint32_t nChannels;                /**< Number of channels (e.g. 2 for stereo) */
   uint32_t nBitPerSample;            /**< Bit per sample */
   uint32_t nSamplingRate;            /**< Sampling rate of the source data.  Use 0 for
                                           variable or unknown sampling rate. */
   uint32_t bFlags;                   /**< Bitfield composed of combined MDEC_PCM_FLAGS values */
} MDEC_PCM_CONFIG;


/** DDP params *****************************************************************************************/

typedef enum MDEC_DDP_BITSTREAMID {
   MEDIA_DEC_DDPBitStreamIdAC3 = 8,
   MEDIA_DEC_DDPBitStreamIdEAC3 = 16,
} MDEC_DDP_BITSTREAMID;

typedef enum MDEC_DDP_BITSTREAMMODE {
   MEDIA_DEC_DDPBitStreamModeCM = 0,   /**< DDP any main audio service: complete main (CM) */
   MEDIA_DEC_DDPBitStreamModeME,       /**< DDP any main audio service: music and effects (ME) */
   MEDIA_DEC_DDPBitStreamModeVI,       /**< DDP any associated service: visually impaired (VI) */
   MEDIA_DEC_DDPBitStreamModeHI,       /**< DDP any associated service: hearing impaired (HI)  */
   MEDIA_DEC_DDPBitStreamModeD,        /**< DDP any associated service: dialogue (D)           */
   MEDIA_DEC_DDPBitStreamModeC,        /**< DDP any associated service: commentary (C)         */
   MEDIA_DEC_DDPBitStreamModeE,        /**< DDP any associated service: emergency (E)          */
   MEDIA_DEC_DDPBitStreamModeVO,       /**< DDP associated service: voice over (VO)            */
   MEDIA_DEC_DDPBitStreamModeK,        /**< DDP main audio service: karaoke                    */
} MDEC_DDP_BITSTREAMMODE;


typedef struct MEDIA_DEC_PARAM_DDPTYPE
{
   uint32_t nChannels;            /**< Number of channels */
   uint32_t nBitRate;             /**< Bit rate of the input data.  Use 0 for variable
                                     rate or unknown bit rates */
   uint32_t nSampleRate;          /**< Sampling rate of the source data. Use 0 for
                                     variable or unknown sampling rate. */
   uint32_t eBitStreamId;         /** of type MDEC_DDP_BITSTREAMID, kept uint32_t to translate to VC easily */
   uint32_t eBitStreamMode;       /** of type MDEC_DDP_BITSTREAMMODE, ditto */
} MDEC_DDP_CONFIG;


/** DTS params *****************************************************************************************/

typedef struct MEDIA_DEC_PARAM_DTSTYPE
{
   uint32_t nChannels;            /**< Number of channels */
   uint32_t nBitRate;             /**< Bit rate of the input data.  Use 0 for variable
                                     rate or unknown bit rates */
   uint32_t nSampleRate;          /**< Sampling rate of the source data. Use 0 for
                                     variable or unknown sampling rate. */
   uint32_t nDtsType;             /** nDtsType=1 for now, FIXME **/
} MDEC_DTS_CONFIG;


/** Flags for buffer passing ***************************************************************************/

typedef enum
{
   MEDIA_DEC_FLAGS_NONE          = 0,
   MEDIA_DEC_FLAGS_END_OF_STREAM = 1 << 0,
   MEDIA_DEC_FLAGS_STARTTIME     = 1 << 1,
   MEDIA_DEC_FLAGS_TIME_UNKNOWN  = 1 << 2,
   MEDIA_DEC_FLAGS_FRAGMENTS     = 1 << 3,  ///< the buffer contains a fragment list rather than data. Used for "zero" copy
   MEDIA_DEC_FLAGS_CODEC_CONFIG  = 1 << 4,  // Used for passing WMV/WMA codec header inband
   MEDIA_DEC_FLAGS_DISCONTINUITY = 1 << 5,  ///< the buffer is the first in a new stream - used for sgement content with different parameters
   MEDIA_DEC_FLAGS_CALLBACK      = 1 << 15, ///< requests a callback when the frame is rendered
} MEDIA_DEC_FLAGS_T;

/** Fragment definition for MEDIA_DEC_FLAGS_FRAGMENTS interpretation of a buffer's payload *************/

typedef struct
{
   uint32_t Physical;
   uint32_t Bytes;
} MEDIA_DEC_FRAGMENT;

/** Enums for display region use ***********************************************************************/

typedef enum
{
   MEDIA_DEC_REGION_MODE_FILL            = 0,    ///< fill mode
   MEDIA_DEC_REGION_MODE_LETTERBOX       = 1,    ///< letterbox mode
   MEDIA_DEC_REGION_MODE_ASPECT_LOCKED   = 0,    ///< preserve the aspect ratio
   MEDIA_DEC_REGION_MODE_ASPECT_UNLOCKED = 1<<4, ///< allow the aspect ratio to change
} MEDIA_DEC_ASPECT_FLAGS_T;

typedef enum
{
   MEDIA_DEC_REGION_TRANSFORM_ROT0          = 0,
   MEDIA_DEC_REGION_TRANSFORM_MIRROR_ROT0   = 1,
   MEDIA_DEC_REGION_TRANSFORM_MIRROR_ROT180 = 2,
   MEDIA_DEC_REGION_TRANSFORM_ROT180        = 3,
   MEDIA_DEC_REGION_TRANSFORM_MIRROR_ROT90  = 4,
   MEDIA_DEC_REGION_TRANSFORM_ROT270        = 5,
   MEDIA_DEC_REGION_TRANSFORM_ROT90         = 6,
   MEDIA_DEC_REGION_TRANSFORM_MIRROR_ROT270 = 7,
} MEDIA_DEC_TRANSFORM_T;


#endif // _mdec_codecs_h
