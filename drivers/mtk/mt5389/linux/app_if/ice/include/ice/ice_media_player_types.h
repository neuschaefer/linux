/**
 * Copyright 2007 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for file playback types
 *
 * This file defines the ICE structures used by the ice_file_playback interface
 *
 */

#ifndef CABOT_ICE_ICE_MEDIA_PLAYER_TYPES_H_INCLUDED
#define CABOT_ICE_ICE_MEDIA_PLAYER_TYPES_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "frost/frost_basictypes.h"


/**
 * Constant that represents an invalid/unknown stream index.
 */
#define ICE_MEDIA_PLAYER_INVALID_STREAM_INDEX (-1)


/**
 * @brief Opaque type used to identify an open file playback system.
 */
typedef struct ICE_MediaPlayerHandle ICE_MediaPlayerHandle;


/**
 * Bitwise OR'able enumeration of possible stream types in a media file.
 */
typedef enum ICE_MediaPlayerStreamType
{
   /**
    * A stream containing video data.
    */
    ice_media_player_stream_type_video,

   /**
    * A stream containing audio data.
    */
    ice_media_player_stream_type_audio,

   /**
    * A stream containing surround audio data.
    */
    ice_media_player_stream_type_surround_audio,

   /**
    * A stream containing surround audio data.
    */
    ice_media_player_stream_type_surround_plus_audio,

   /**
    * A stream containing subtitle data.
    */
    ice_media_player_stream_type_subtitle,

   /**
    * A stream containing still image data.
    */
    ice_media_player_stream_type_still_image,

   /**
    * The maximum number of stream types.
    */
    ice_media_player_stream_type_max,

   /**
    * An unknown stream type.
    */
    ice_media_player_stream_type_unknown = ice_media_player_stream_type_max

} ICE_MediaPlayerStreamType;


/**
 * @brief Representation of the details of a stream in a file.
 */
typedef struct ICE_MediaPlayerStreamDetails
{
    /**
     * The type of this stream.
     */
    ICE_MediaPlayerStreamType type;

    /**
     * The index of this stream among other streams of the same
     * type in the same file.
     */
    frost_uint8 index;

    /**
     * The index of this stream among other streams of the same
     * type in the same file.
     */
    frost_uint8 codec;
    
    /**
     * Pointer to null-terminated string containing the stream's description.
     *
     * This may contain the language of the stream and will typically be
     * displayed by an application on the screen to help end-users select
     * the streams they would like to use.
     */
    char * description;

} ICE_MediaPlayerStreamDetails;


/**
 * @brief Representation of the details of a media file.
 */
typedef struct ICE_MediaPlayerFileDetails
{
    /**
     * Duration of the file in seconds. Null if unknown.
     */
    frost_uint32 duration_sec;

    /**
     * Pointer to null-terminated string containing the file's artist.
     * Null if unknown.
     */
    char * artist;

    /**
     * Pointer to null-terminated string containing the file's album.
     * Null if unknown.
     */
    char * album;

    /**
     * Pointer to null-terminated string containing the file's year.
     * Null if unknown.
     */
    char * year;

    /**
     * Pointer to null-terminated string containing the file's genre.
     * Null if unknown.
     */
   char * genre;

   /**
     * Pointer to null-terminated string containing the file's title.
     * Null if unknown.
     */
    char * title;

    /**
     * Pointer to null-terminated string containing the file's description.
     * Null if unknown.
     */
    char * description;

    /**
     * Array of details for the streams contained in the file.
     */
    ICE_MediaPlayerStreamDetails ** streams;

    /**
     * Number of streams contained in the #streams array.
     */
    frost_uint8 number_of_streams;

    /**
     * Flag for support of utf8 characters in media details.
     */
    frost_bool utf8_support;

    /**
     * Flag for support of range in media details.
     */
    frost_bool range_support;

    /**
     * Bit rate of media
     */
    frost_uint32 bit_rate;

} ICE_MediaPlayerFileDetails;


/**
 * Enumeration of possible media file playback events passed to the
 * ICE_MediaPlayerStatusCallback.
 */
typedef enum ICE_MediaPlayerPlaybackEvent
{
   /**
    * Playback device is now playing at least one of the selected streams after
    * a successful call to ICE_MediaPlayerPlay().
    */
    ice_media_player_play_event_started,

   /**
    * Playback device has completed the stop operation after a successful
    * call to ICE_MediaPlayerStop().
    */
    ice_media_player_play_event_stopped,

   /**
    * Playback device has reached the beginning of the file when in reverse
    * playback mode.
    *
    * Prior to calling the callback, the ICE implementation must set the
    * playback speed to 0 (as if ICE_MediaPlayerSetSpeed(0) had been called),
    * so the playback is in a paused state.
    */
    ice_media_player_play_event_reached_beginning,

   /**
    * Playback device has reached the end of the file when in forward playback
    * mode.
    *
    * Prior to calling the callback, the ICE implementation must set the
    * playback speed to 0 (as if ICE_MediaPlayerSetSpeed(0) had been called),
    * so the playback is in a paused state.
    */
    ice_media_player_play_event_reached_end,

   /**
    * Playback device can not decode audio. 
    * 
    * After calling the callback, user should informed about audio support.
    * Playing the file continoues and state remains same.
    */
    ice_media_player_play_event_unsupported_audio,
   
    ice_media_player_play_event_unsupported_resolution,

    ice_media_player_play_event_unsupported_framerate,

    ice_media_player_play_event_unsupported_feature, 

    ice_media_player_play_event_unauthorised_user,
    
    ice_media_player_buffer_low,

    ice_media_player_buffer_ok,

   /**
    * Playback device has encountered an irrecoverable error playing back
    * or downloading the file. Examples of such errors are read errors,
    * timeouts during a download, or incompatible file formats.
    *
    * Resources are not deallocated until <code>ICE_MediaPlayerClose</code> is
    * called.
    */
    ice_media_player_play_event_error

} ICE_MediaPlayerPlaybackEvent;


/**
 * Enumeration of possible subtitle types passed to the
 * ICE_MediaPlayerSubtitleCallback.
 */
typedef enum ICE_MediaPlayerSubtitleType
{
   /**
    * A subtitle which is composed only of textual data.
    */
    ice_media_player_subtitle_type_textual,

    /**
     * A subtitle which is composed only of textual data of utf8.
     */
    ice_media_player_subtitle_type_textual_utf8

} ICE_MediaPlayerSubtitleType;


/**
 * Callback function to inform Eclipse of a media player event during playback.
 *
 * @param event         The ICE_MediaPlayerPlaybackEvent generated.
 * @param stream_type   The type of the stream with which the event is related,
 * or ice_media_player_stream_type_unknown if the event is not related to any
 * specific stream.
 * @param user_data     The user_data parameter passed to ICE_MediaPlayerPlay().
 */
typedef void (*ICE_MediaPlayerStatusCallback)(ICE_MediaPlayerPlaybackEvent event,
                                              ICE_MediaPlayerStreamType stream_type,
                                              void* user_data);

typedef void (*ICE_MediaPlayerDetailsCallback)(ICE_MediaPlayerFileDetails * details,
                                                void* user_data);

/**
 * Callback function to inform Eclipse that a subtitle must be displayed.
 *
 * @param data          Contents of the subtitle packet. When carrying textual
 *                      data, this does not have to be null-terminated.
 * @param data_length   Length of the data passed in through the data parameter.
 * @param duration_msec Duration, in milliseconds, for which the subtitles should
 *                      be kept on the screen.
 * @param user_data     The user_data parameter passed to ICE_MediaPlayerPlay().
 */
typedef void (*ICE_MediaPlayerSubtitleCallback)(ICE_MediaPlayerSubtitleType type,
                                                frost_uint8 const * data,
                                                frost_uint32 data_length,
                                                frost_uint32 duration_msec,
                                                void* user_data);

/**
 * Enumeration of possible media player status
 */
typedef enum ICE_MediaPlayerStatus
{
    ICE_MediaPlayerStatusSuccess,
    ICE_MediaPlayerStatusFailure,
    ICE_MediaPlayerStatusTrickModeUnsupported
}ICE_MediaPlayerStatus;


/**
 * @brief Representation of buffering status for a streamed media file.
 */
typedef struct ICE_MediaPlayerBufferingStatus
{
    /**
     * total length of the streamed content in kilobytes.
     */
    frost_uint32 content_length_in_kbytes;
    
    /**
     * the amount of data downloaded so far in kilobytes.
     */
    frost_uint32 downloaded_amount_in_kbytes;

    /**
     * an offset from start of media in seconds.
     */
    frost_uint32 offset_from_start_in_secs; 

    /**
     * an offset from end of media in seconds.
     */
    frost_uint32 offset_from_end_in_secs; 

    /**
     * whether or not media is being downloaded
     */
    frost_bool downloading_in_progress;
}ICE_MediaPlayerBufferingStatus;

/**
 * Enumeration of media player play parameters
 */
typedef enum ICE_MediaPlayerPlayParam
{
    /**
     * Parameter to set user agent in http get requests
     * type: (char*)
     */
    ice_mp_param_http_user_agent,
    /**
     * Additional headers in http get requests
     * type: (char*)
     */
    ice_mp_param_http_additional_headers,
    /**
     * Parameter to enable buffer monitoring
     * type: (frost_bool)
     */
    ice_mp_param_buffer_monitor,
    /**
     * Parameter to enable precharge network buffer
     * type: (frost_bool)
     */
    ice_mp_param_precharge_buffer,
    /**
    * Parameter to set starting point of the media in seconds
     * type: (frost_uint32)
     */
    ice_mp_param_play_offset,
    /**
    * Parameter to set network socket timeout value.
    * Socket reader should wait until media player reaches this value.
     * type: (frost_uint32)
     */
    ice_mp_param_network_timeout
}ICE_MediaPlayerPlayParam;

/**
 * Enumeration for Driver Divx error codes.
 */
typedef enum ICE_DivxErrorCodes
{  
    ice_divx_success,
    ice_divx_not_supported_screen_size,    /**< The file has not supported screen size. */
    ice_divx_not_supported_frame_rate,    /**< The file has not supported frame rate. */ 
} ICE_DivxErrorCodes;

/**
 * Enumeration for media player error codes.
 */
typedef enum ICE_MediaPlayerError
{
    ice_media_player_error_none = 0,
    ice_media_player_error_av_format_not_supported = 1,
    ice_media_player_error_can_not_connect_or_connection_lost = 2,
    ice_media_player_error_insufficient_resources = 3,
    ice_media_player_error_content_corrupt_or_invalid = 4,
    ice_media_player_error_content_not_available = 5,
    ice_media_player_error_content_not_available_at_position = 6,
    ice_media_player_error_unidentified = 7
}ICE_MediaPlayerError;


#ifdef __cplusplus
}
#endif


#endif /* CABOT_ICE_ICE_MEDIA_PLAYER_TYPES_H_INCLUDED */
