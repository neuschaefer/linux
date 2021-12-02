/**
 * Copyright 2007 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief ICE interface for media file playback.
 */ 

#ifndef CABOT_ICE_ICE_MEDIA_PLAYER_H_INCLUDED
#define CABOT_ICE_ICE_MEDIA_PLAYER_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_media_player_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Attempts to open a connection to a media player subsystem, and returns a handle 
 * for subsequent communication with the device.
 *
 * @return Returns a pointer to the handle to a media player device on success and a 
 * NULL pointer on failure.
 */
ICE_MediaPlayerHandle * ICE_MediaPlayerOpen(void);


/**
 * @brief Close a connection to a media player.
 *
 * Closes a previously opened connection to the media player identified.
 *
 * @param handle The handle to the required media player.
 */
void ICE_MediaPlayerClose(ICE_MediaPlayerHandle* handle);

/**
 * @brief Query the module to see if the module is busy now.
 *
 * @return frost_true if the module is in use, false otherwise.
 */
frost_bool ICE_IsMediaPlaybackInProgress();

/**
 * @brief Get the details of a media file.
 *
 * @param handle    The handle to the required media player.
 * @param filespec  An absolute drive, partition, pathname and filename 
 *                  specifier for a local file, or a URL for a remote file.
 * @param object  Object of media playerlist
 * @param include_stream_details Whether the details of every stream should
 *                  be included in the returned ICE_MediaPlayerFileDetails structure 
 *                  or not. When set to false, the <code>streams</code> field of the 
 *                  returned structure is expected to be null.
 *
 * @return          Pointer to structure in which the details of the file
 *                  will be returned.  Allocates memory which must be released by
 *                  calling #ICE_MediaPlayerReleaseFileDetails. NULL if the
 *                  file details are unknown.
 */
ICE_MediaPlayerFileDetails* ICE_MediaPlayerGetFileDetails(ICE_MediaPlayerHandle* handle,
                                                          char const* filespec,
                                                          void * object,
                                                          frost_bool include_stream_details);

/**
 * @brief Release the resources allocated during a successful call to 
 *        #ICE_MediaPlayerGetFileDetails
 *
 * @param handle            The handle to the required media player.
 * @param file_details    Pointer to the file details structure that is no longer used.
 */
void ICE_MediaPlayerReleaseFileDetails(ICE_MediaPlayerHandle* handle,
                                       ICE_MediaPlayerFileDetails* file_details);

/**
 * @brief Get the total playback time for a handle
 *
 * Some platforms can supply time information about media file after stream selection and playback.
 * So let them give this information via this function. If time is unknown at the moment or can not be determined, return zero.
 * If file is not opened, parsed before or not playing, function may return zero. It will not try to re-open and parse file. 
 * 
 * @param handle    The handle to the required media player.
 *
 * @return          The total playback time in seconds..
 */
frost_uint32 ICE_MediaPlayerGetTotalPlaybackTime(ICE_MediaPlayerHandle* handle);


/**
 * @brief Select a stream for playback.
 *
 * If the player is stopped, this function will prepare the player to play the selected
 * streams when #ICE_MediaPlayerPlay is called.
 *
 * If the player is already playing, this function will change the stream being played
 * without interrupting playback.
 *
 * This function will be called as many times as necessary to play all the required
 * streams from one or more files.
 *
 * Example: To play the first video stream and the first audio stream from a file,
 * the following function calls may occur:
 *
 * <code>
 * ICE_MediaPlayerSetStream(handle, "foo.mpg", ice_media_player_stream_video, 0);
 * ICE_MediaPlayerSetStream(handle, "foo.mpg", ice_media_player_stream_audio, 0);
 * ICE_MediaPlayerPlay(handle);
 * </code>
 *
 * @note This does not check whether the file actually contains a stream with the selected
 * stream_type and stream_index. (e.g. if the video stream at index 0 is selected, but
 * the file does not contain one, then no video will be shown). Use #ICE_MediaPlayerGetFileDetails
 * to access the actual details of the streams within a file.
 *
 * @param handle                Hadle to the required media file subsystem.
 * @param filespec              An absolute drive, partition, pathname and filename 
 *                              specifier for a local file, or a URL for a remote file.
 * @param stream_type           The type of the stream.
 * @param object  Object of media playerlist
 * @param stream_index          Index of the stream to be played among streams of the same type.
 *                              The special value of #ICE_MEDIA_PLAYER_INVALID_STREAM_INDEX will
 *                              disable the playback of streams of the given type.
 *
 * @return frost_true if the source has been set, false otherwise.
 */
frost_bool ICE_MediaPlayerSelectStream(ICE_MediaPlayerHandle* handle,
                                       char const* filespec,
                                       ICE_MediaPlayerStreamType stream_type,
                                       void * object,
                                       frost_int stream_index);


/**
 * @brief Start playback of specific streams in specific file(s)
 *
 * This function must return immediately and status updates must be signalled 
 *
 * @param handle                Handle to the required media file subsystem.
 * @param status_callback       The callback function to be called in the case of a 
 *                              status update during playback.
 * @param subtitle_callback     The callback function to be called when a new subtitle
 *                              event occurs.
 * @param resume  define  playback is resumed or not
 * @param user_data             Pointer to data that should be used as the parameter
 *
 * @return frost_true if the source has been set, false otherwise.
 */
frost_bool ICE_MediaPlayerPlay(ICE_MediaPlayerHandle* handle,
                               ICE_MediaPlayerStatusCallback status_callback,
                               ICE_MediaPlayerDetailsCallback media_details_callback,
                               ICE_MediaPlayerSubtitleCallback subtitle_callback,
                               frost_bool resume,
                               void* user_data);

/**
 * @brief Query for if media file able to support rewind/jump function.
 *
 * This function returns rewind/jump functions are able to use current media file or not.
 *
 * @param handle The handle to the required video file.
 *
 * @return #frost_true if supports, #frost_false if not supports.
 */
frost_bool ICE_MediaPlayerIsRewindSupported(ICE_MediaPlayerHandle* handle);                               


/**
 * @brief Stop playback of a media file.
 *
 * This function stops the playing of a video file and releases all resources 
 * allocated for it.
 *
 * @param handle The handle to the required video file. Becomes invalid after
 *        a successful call to this function and must not be used again.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_MediaPlayerStop(ICE_MediaPlayerHandle* handle);

/**
 * @brief Set the number of seconds into the playback.
 *
 * This is the number of seconds from the start of the file.
 *
 * An "out of range" value sets the position to the closest valid value.
 *
 * @param handle Handle to the current playback.
 * @param seconds The number of seconds into the playback.
 * @return frost_true if the playback point has been set correctly, 
 *         otherwise frost_false.
 */
frost_bool ICE_MediaPlayerSetPosition(ICE_MediaPlayerHandle *handle, 
                                      frost_uint32 seconds);

/**
 * @brief Get the number of seconds into the playback.
 *
 * The playback position is represented by the number of seconds
 * into the playback.
 *
 * @param handle Handle to the current playback.
 * @return The number of seconds into the playback.
 */
frost_uint32 ICE_MediaPlayerGetPosition(ICE_MediaPlayerHandle *handle);

/**
 * @brief Set the playback speed
 *
 * @param handle Handle to the current playback.
 * @param speed_per_thousandths Playback speed in per-thousandths.  Eg. 2000 is double speed,
 *        500 is half speed, Pause is zero, -3000 is triple speed in reverse. 0 freezes the 
 *        current frame, although the implementation may choose to continue to buffer the data
 *        when playing a remote file from a network.
 * @note  The implementation will use the nearest supported speed to the 
 *        speed requested by this function.
 * @return frost_true if the playback speed has been adjusted to be as close 
 *         as possible to the requested speed, otherwise frost_false.
 */
frost_bool ICE_MediaPlayerSetSpeed(ICE_MediaPlayerHandle *handle, 
                                   frost_int32 speed_per_thousandths);

/**
 * @brief Get the current playback speed.
 *
 * @note This function retrieves the actual playback speed which may differ
 * from the request to ICE_MediaPlayerSetSpeed().
 *
 * @param handle Handle to the current playback.
 * @return The current playback speed in per-thousandths.
 *        Eg. 2000 is double speed, 500 is half speed,
 *        Pause is zero, -3000 is triple speed in reverse.
 */
frost_int32 ICE_MediaPlayerGetSpeed(ICE_MediaPlayerHandle *handle);

/**
 * @brief Set the time correction value for a specific stream type.
 *
 * At the beginning of playback, all streams are played with a time 
 * correction value of 0, i.e. packets are presented at their original 
 * presentation times. When this function successfully returns,
 * packets of the given stream type will be shifted backwards or 
 * forwards in time, depending on the polarity of the milliseconds
 * parameter. This is particularly useful for subtitle packets read
 * from an external source, when the subtitle presentation times
 * are not in sync with audio and video.
 *
 * @param handle Handle to the current playback.
 * @param stream_type Type of the stream whose packets require time correction.
 * @param milliseconds The number of milliseconds by which the packets 
 *        belonging to the given stream_type must be shifted in time. May be 
 *        negative.
 * @return frost_true if the time correction has been set correctly, 
 *         otherwise frost_false.
 */
frost_bool ICE_MediaPlayerSetTimeCorrection(ICE_MediaPlayerHandle *handle,
                                            ICE_MediaPlayerStreamType stream_type,
                                            frost_int32 milliseconds);

/**
 * @brief Set status of AV sync (enabled or disabled)
 *
 * @param enable frost_true if we enable AV sync of media player frost_false if it is disabled
 */
void ICE_MediaPlayerSetAvSync(frost_bool enable);

/**
 * @brief Return the current status of the buffer that is used to cache
 *        streamed content.
 *
 * @param handle Handle to the current playback.
 * @param status Status structure to be filled by this function.
 *
 * @return frost_true if status is gathered succesfully, frost_sfalse otherwise.
 */
frost_bool ICE_MediaPlayerGetBufferingStatus(ICE_MediaPlayerHandle* handle,
                                             ICE_MediaPlayerBufferingStatus* status); 

/**
 * @brief Checks the allocated media stream buffer in use, if it has cached data or not.
 *
 * @param handle Handle to the current playback.
 *
 * @return frost_true if buffer cached is emthy, false otherwise.
 */
frost_bool
ICE_MediaPlayerIsBufferCacheEmpthy(ICE_MediaPlayerHandle* handle);

/**
 * @brief Interface to set media player configuration like user agent.
 *
 * @param handle Handle to the current playback.
 * @param param Parameter to configure in media player.
 * @param value Value to set to the parameter.
 *
 * @return frost_true if set operation successfull, frost_false otherwise 
 */
frost_bool 
ICE_SetMediaPlayerParam(ICE_MediaPlayerHandle* handle,ICE_MediaPlayerPlayParam param, const void* value);

/**
*@brief checks the played video is divx or not
*@return  true if playing video is DivX.
*/
frost_bool 
ICE_MediaPlayerGetCodecIsDivx();


/**
 * @brief Browser asks MP if it can play a specific type and codec.
 * Codecs is the additional information added in the <video/audio> tag.
 *
 * @details Will be called multiple times with different values
 * if multiple sources.
 *
 * @params type Mimetype of the media source.
 * @params argc Number of arguments in the array of codecs.
 * @params codecs Array of codecs for this type.
 *
 * @returns frost_true if this type is supported.
 *
 * @returns frost_false otherwise.
 */
frost_bool
ICE_MediaPlayerCanPlayType(const frost_int8 * type, frost_int argc, const frost_int8 * codecs[]);

/**
 * @brief Get the error condition happened.
 *
 * @note This function retrieves the error condition happened
 *
 * @param handle Handle to the current playback.
 * @return The current error condition
 */
ICE_MediaPlayerError 
ICE_MediaPlayerGetError(ICE_MediaPlayerHandle *handle);

void ICE_MediaPlayerGetStreamerFileDetails( const ICE_MediaPlayerFileDetails * streamer_file_details);
#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_MEDIA_PLAYER_H_INCLUDED */
