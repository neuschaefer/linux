/**
 * Copyright 2006 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for Video types
 *
 * This file defines the ICE structures used by the ice_video interface
 *
 */

#ifndef CABOT_ICE_ICE_VIDEO_TYPES_H_INCLUDED
#define CABOT_ICE_ICE_VIDEO_TYPES_H_INCLUDED

#include "ice/ice_device_number.h"
#include "ice/ice_display_types.h"

/**
 * PAL standard width.
 */
#define SCREEN_WIDTH_PAL        720

/**
 * PAL standard height.
 */
#define SCREEN_HEIGHT_PAL       576

/**
 * PAL standard x offset.
 */
#define SCREEN_X_OFFSET_PAL     132

/**
 * PAL standard y offset.
 */
#define SCREEN_Y_OFFSET_PAL     23

/**
 * PAL standard frame rate.
 */
#define FRAME_RATE_PAL          50

/**
 * PAL M standard frame rate.
 */
#define FRAME_RATE_PAL_M        60

/**
 * NTSC standard width.
 */
#define SCREEN_WIDTH_NTSC       720

/**
 * NTSC standard height.
 */
#define SCREEN_HEIGHT_NTSC      480

/**
 * NTSC standard x offset.
 */
#define SCREEN_X_OFFSET_NTSC    122

/**
 * NTSC standard y offset.
 */
#define SCREEN_Y_OFFSET_NTSC    20

/**
 * NTSC standard frame rate.
 */
#define FRAME_RATE_NTSC         60


#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************************/
/**
 * @name Type Definitions
 * @{
 */
/***************************************************************************/

/**
 * @brief Opaque type used to identify an open connection.
 */
typedef struct ICE_VideoHandle ICE_VideoHandle;

/**
 * The types of status messages that can be sent by the ICE implementation
 * to Eclipse.
 */
typedef enum ICE_VideoStatus {

    /**
     * Send when the first frame is decoded and ready to be displayed after
     * a call to ICE_VideoStart().
     *
     * Note that this is the FIRST frame - do NOT send a callback on every
     * frame!
     */
    ICE_VideoStatusFirstFrameDecoded,

    /**
     * Send when the video attributes change.
     *
     * Video attributes are decode width/height, aspect ratio, and AFD.
     *
     * The driver level software should call the status callback and pass
     * this code on detection of a change in video aspect ratio, AFD (Active
     * Format Descriptor), width or height for the currently decoding video
     * stream or I-frame.  Following a call to this callback, Eclipse will
     * determine the video attributes via calls to ICE_DecodeWidth(),
     * ICE_DecodeHeight(), ICE_DecodeAspectRatio() and ICE_DecodeAFD() and
     * will set the video window accordingly using ICE_VideoPosition().
     */
    ICE_VideoStatusAttributesChanged,

    /**
     * Send when the video decoder could not be started after a call to
     * ICE_VideoStart().
     */
    ICE_VideoStatusDecodingError,

    /**
     * Send when analog audio is ready to be displayed after
     * a call to ICE_VideoStart().
     */
    ICE_AnalogVideoStatusVideoReady,
    
    /**
     * Send when the video attributes change for analog signal.
     */
    ICE_AnalogVideoStatusAttributesChanged,

    /**
     * Send when the video from the external sources is ready to be displayed after
     * a call to ICE_VideoStart().
     */
    ICE_ExternalVideoStatusVideoReady,
    
    /**
     * Send when the video signal is lost for an external interface
     * although there is a connected video input on that interface.
     */
    ICE_ExternalVideoStatusVideoLost,

    /**
     * Send when the video attributes change for analog signal.
     */
    ICE_ExternalVideoStatusAttributesChanged,

    /**
     * Send when the auto adjustment process is completed for external sources like Ypbpr, VGA (PC)...
     */
    ICE_ExternalVideoStatusAutoAdjustCompleted,

    /**
     * Send when the video decoder has started receiving Frames after ICE_VideoStatusDecodingStopped
     */
    ICE_VideoStatusDecodingRestarted,

    /**
     * Send when the video decoder has stopped receiving Frames
     */
    ICE_VideoStatusDecodingStopped

} ICE_VideoStatus;

/**
 * @brief Callback function to inform Eclipse of the video decoder status.
 *
 * See the comments in ICE_VideoStatus for when this callback should be
 * called.
 *
 * @param user_data As supplied in the call to ICE_VideoOpen().
 * @param status The video decoder status.
 */
typedef void (*ICE_VideoStatusCallback)(void* user_data, ICE_VideoStatus status);

/**
 * @brief Enumeration of possible video encoding modes.  These are also known as
 *        "colour standards" in analogue broadcasting world.
 *
 * @note It is anticipated that any given ICE implementation will only support
 *       a subset of the modes below.
 */
typedef enum ICE_VideoEncodingMode
{
    /**
     * PAL I mode                  - UK and Ireland
     */
    ICE_video_encoding_mode_pal_i        = 0x01,

    /**
     * PAL B, G, and H modes       - Europe, Asia
     */
    ICE_video_encoding_mode_pal_bgh      = 0x02,

    /**
     * PAL D mode                  - China
     */
    ICE_video_encoding_mode_pal_d        = 0x04,

    /**
     * PAL N mode                  - Paraguay, Argentina
     */
    ICE_video_encoding_mode_pal_n        = 0x08,

    /**
     * PAL M mode                  - Brazil
     */
    ICE_video_encoding_mode_pal_m        = 0x10,

    /**
     * NTSC M mode                 - North America, Taiwan, Japan, Mexico,
     *                               Canada, Korea, Philippines
     */
    ICE_video_encoding_mode_ntsc_m       = 0x20,

    /**
     * NTSC mode
     * 
     */
    ICE_video_encoding_mode_ntsc            = 0x40,

    /**
     * SECAM B, G and H mode       - Middle East, N. Africa
     */
    ICE_video_encoding_mode_secam_bgh    = 0x80,

    /**
     * SECAM D, K, K1 and L mode   - East Europe, Russia
     */
    ICE_video_encoding_mode_secam_dkk1l  = 0x100

} ICE_VideoEncodingMode;


/**
 * @brief Representation of an aspect ratio.
 *
 * This is the aspect ratio of a video or I-frame as transmitted.
 * This is the value of the aspect_ratio_information field in the MPEG video
 * data.  (Note that the enum values are defined to be identical to the
 * MPEG bit-patterns, to make this easy to implement by just reading
 * the field and casting it to this enum).
 *
 * Implementations are not required to be able to return all these aspect
 * ratio values - only ICE_aspect_ratio_4_3 and ICE_aspect_ratio_16_9
 * are important.
 */
typedef enum ICE_VideoAspectRatio
{

    ICE_aspect_ratio_unknown        = -1, /**< Typically only used if playback
                                               has not yet been started */

    ICE_aspect_ratio_forbidden_0    = 0x0,
    ICE_aspect_ratio_square_samples = 0x1, /**< Same as coded frame */
    ICE_aspect_ratio_4_3            = 0x2, /**< 4:3 */
    ICE_aspect_ratio_16_9           = 0x3, /**< 16:9 */
    ICE_aspect_ratio_2_21_1         = 0x4, /**< 2.21:1 */
    ICE_aspect_ratio_reserved_5     = 0x5,
    ICE_aspect_ratio_reserved_6     = 0x6,
    ICE_aspect_ratio_reserved_7     = 0x7,
    ICE_aspect_ratio_reserved_8     = 0x8,
    ICE_aspect_ratio_reserved_9     = 0x9,
    ICE_aspect_ratio_reserved_10    = 0xa,
    ICE_aspect_ratio_reserved_11    = 0xb,
    ICE_aspect_ratio_reserved_12    = 0xc,
    ICE_aspect_ratio_reserved_13    = 0xd,
    ICE_aspect_ratio_reserved_14    = 0xe,
    ICE_aspect_ratio_reserved_15    = 0xf
} ICE_VideoAspectRatio;


/**
 * @brief Enumeration of possible Active Format Descriptors (AFD's) in
 *        MPEG user data.
 *
 * Note that this is the full 4-bit AFD.  In the AFDs defined by the UK
 * D-book, the top bit is always set to '1'.  (The UK D-book issue 3.2
 * refers to these AFDs using just the low 3 bits, as "AFD 0" through
 * "AFD 7", but these are actually AFDs 1000b = 8 through 1111b = 15 in
 * this enum.  The UK D-Book issue 4.0 does not refer to these confusing
 * numbers, and uses the full binary bit-pattern instead.)
 *
 * The AFDs with the top bit set to '0' are defined by ETR154 / EACEM TR-030.
 *
 * For UK D-book compliance it is acceptable to only return values 8-15,
 * and to return ICE_afd_same_as_coded_frame instead of
 * ICE_afd_not_present.  (I.e. if you are porting an old implementation of
 * ICE_DecodeAFD() to the new API you can just add 8 to the previous return
 * value).  However, for compatibility with non-UK markets it is recommended
 * to fully implement this API.
 *
 * (Note that the enum values are defined to be identical to the MPEG data
 * bit-patterns, to make this easy to implement by just reading the field
 * and casting it to this enum).
 */
typedef enum ICE_VideoAFD
{
    ICE_afd_not_present                 = -1,  /**< AFD not present, or AFD not yet found. */

    ICE_afd_reserved_0                  = 0x0, /**< AFD 0000 - Reserved. */
    ICE_afd_reserved_1                  = 0x1, /**< AFD 0001 - Reserved. */
    ICE_afd_16_by_9_top                 = 0x2, /**< AFD 0010 - box 16:9 (top). */
    ICE_afd_14_by_9_top                 = 0x3, /**< AFD 0011 - box 14:9 (top). */
    ICE_afd_gt_16_by_9                  = 0x4, /**< AFD 0100 - box >16:9 (centre). */
    ICE_afd_reserved_5                  = 0x5, /**< AFD 0101 - Reserved. */
    ICE_afd_reserved_6                  = 0x6, /**< AFD 0110 - Reserved. */
    ICE_afd_reserved_7                  = 0x7, /**< AFD 0111 - Reserved. */

    ICE_afd_same_as_coded_frame         = 0x8, /**< AFD 1000 - Same as coded frame. */
    ICE_afd_4_by_3                      = 0x9, /**< AFD 1001 - 4:3. */
    ICE_afd_16_by_9                     = 0xa, /**< AFD 1010 - 16:9. */
    ICE_afd_14_by_9                     = 0xb, /**< AFD 1011 - 14:9. */
    ICE_afd_reserved_12                 = 0xc, /**< AFD 1100 - Reserved. */
    ICE_afd_4_by_3_with_14_by_9_centre  = 0xd, /**< AFD 1101 - 4:3 with shoot and protect 14:9 centre. */
    ICE_afd_16_by_9_with_14_by_9_centre = 0xe, /**< AFD 1110 - 16:9 with shoot and protect 14:9 centre. */
    ICE_afd_shoot_and_protect_4_by_3    = 0xf  /**< AFD 1111 - 16:9 with shoot and protect 4:3 centre. */
} ICE_VideoAFD;

/**
 * @brief Enumeration of possible Wide Screen Signalling (WSS) aspect
 *        ratio codes, as per ETS 300 294.
 */
typedef enum ICE_WSS_AspectRatioCode
{                                /* Note: of bits 3210, bit 3 is odd parity */
    ICE_WSS_not_present                 = -1,
    ICE_WSS_4_3_full_format             = 0x8, /* 1000 */
    ICE_WSS_16_9_letterbox_top          = 0x4, /* 0100 */
    ICE_WSS_14_9_letterbox_top          = 0x2, /* 0010 */
    ICE_WSS_14_9_full_format_centre     = 0xe, /* 1110 */
    ICE_WSS_14_9_letterbox_centre       = 0x1, /* 0001 */
    ICE_WSS_gt_16_9_letterbox_centre    = 0xd, /* 1101 */
    ICE_WSS_16_9_letterbox_centre       = 0xb, /* 1011 */
    ICE_WSS_16_9_full_format_anamorphic = 0x7  /* 0111 */

} ICE_WSS_AspectRatioCode;


/**
 * @brief Enumeration of possible subtitle Wide Screen Signalling (WSS), as per ETS 300 294.
 */
typedef enum ICE_WSS_Subtitles
{
    ICE_WSS_subtitles_none               = 0,
    ICE_WSS_subtitles_inside_image_area  = 1,
    ICE_WSS_subtitles_outside_image_area = 2,
    ICE_WSS_subtitles_reserved           = 3
} ICE_WSS_Subtitles;


/**
 * @brief Enumeration of video encoding.
 */
typedef enum ICE_VideoStreamType
{
    ICE_VideoStreamType_unknown = 0,
    ICE_VideoStreamType_mpeg1 = 1,
    ICE_VideoStreamType_mpeg2 = 2,
    ICE_VideoStreamType_h264 = 3
} ICE_VideoStreamType;


/**
 * @brief Enumaration of possible zoom modes supported on a TV set.
 */
typedef enum ICE_VideoTVZoomMode
{
    ICE_zoom_mode_invalid,
    ICE_zoom_mode_auto,
    ICE_zoom_mode_4_3,
    ICE_zoom_mode_14_9,
    ICE_zoom_mode_14_9_zoom,
    ICE_zoom_mode_16_9,
    ICE_zoom_mode_16_9_subtitle,
    ICE_zoom_mode_panoramic,
    ICE_zoom_mode_cinema,//16:9 zoom
    ICE_zoom_mode_full, //16:9 full no overscan
    ICE_zoom_mode_4_3_full,//4:3 full no overscan
    ICE_zoom_mode_panoramic_2,
    ICE_zoom_mode_zoom_pal_plus,
    ICE_zoom_mode_4_3_zoom,
}ICE_VideoTVZoomMode;


/**
 * @brief This structure stores zoom mode and corresponding
 * input and output windows for that zoom mode. 
 */
typedef struct ICE_VideoTVZoomParams
{
    /**
      selects a specific zoom mode considering the given aspect ratio
    */
    ICE_VideoTVZoomMode zoom_mode;

    /**
      describes the zoom input window
    */
    ICE_DisplayWindow* input_window;

    /**
      describes the zoom output window
     */
    ICE_DisplayWindow* output_window;
} ICE_VideoTVZoomParams;


#ifdef __cplusplus
}
#endif


#endif /* CABOT_ICE_ICE_VIDEO_TYPES_H_INCLUDED*/
