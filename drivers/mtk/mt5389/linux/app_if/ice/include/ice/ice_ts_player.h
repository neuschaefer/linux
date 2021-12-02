/**
 * Copyright 2010 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief ICE interface for transport stream playback.
 *
 * This API only needs to be implemented if you want to support
 * Freesat or Freeview Interaction Channel streaming.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 */

#ifndef CABOT_ICE_ICE_TS_PLAYER_H_INCLUDED
#define CABOT_ICE_ICE_TS_PLAYER_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_ts_player_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialise the TS player device.
 *
 * @note Only one device is currently assumed.
 *
 * @note When in use, the device requires a demux dedicated to the purpose of
 *       TS playback and other demux devices must continue processing
 *       normally, e.g. section filtering from a broadcast transport stream.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TSPlayerInit(void);

/**
 * @brief Attempts to open a connection to a TS player subsystem.
 *
 * @return Returns a handle on success and a null pointer on failure.
 */
ICE_TSPlayerHandle * ICE_TSPlayerOpen(void);

/**
 * @brief Close a connection to a TS player.
 *
 * Closes a previously opened connection to the TS player.
 *
 * @param handle The handle to the required TS player.
 */
void ICE_TSPlayerClose(ICE_TSPlayerHandle * handle);

/**
 * @brief Start streaming a transport stream.
 *
 * Begin to request transport stream data to be fed into a demux allowing
 * sections and PES data to be filtered from it.
 *
 * Depending on the application, transport stream data may be acquired from a
 * variety of locations, such as a network connection, hard-disk, or
 * non-volatile memory.
 *
 * This function must return immediately and status updates must be signalled.
 * The request callback is used to supply the player with transport stream
 * data as needed.
 *
 * Callbacks should only be called if playback starts successfully (i.e. this
 * function returns non-zero).  If errors are detected before the
 * function returns, such as #ice_ts_player_event_error, they should be
 * signalled and the return value set to frost_true.
 *
 * After #ice_ts_player_event_started has been received, calls will be made
 * to set the #ICE_DeviceSource to #ICE_source_ts_player for video
 * (i.e. with ICE_VideoSelectSource()), audio, STC, TP data processing and
 * section filtering.  Devices with the source set to #ICE_source_ts_player
 * should be fed from the demux associated with the TS player in a similar
 * manner as if the source was #ICE_source_tuner0 for example.
 * Playback will be paused using ICE_TSPlayerPause() during the initial stream
 * selection process.
 *
 * The transport stream may be encrypted, and this will be handled using
 * the ice_descrambler.h API.
 *
 * @param handle            Handle to the required TS player.
 * @param request_callback  The callback function to be called when more data
 *                          is required by the player.
 * @param status_callback   The callback function to be called in the case of a
 *                          status update during TS streaming.
 * @param user_data         Pointer to data that should be used as a parameter
 *                          to the callbacks.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TSPlayerStart(ICE_TSPlayerHandle * handle,
                             ICE_TSPlayerRequestCallback request_callback,
                             ICE_TSPlayerStatusCallback status_callback,
                             void * user_data);

/**
 * @brief Provide data requested with an #ICE_TSPlayerRequestCallback.
 *
 * @see #ICE_TSPlayerRequestCallback for more details.
 *
 * @param handle            Handle to the required TS player.
 * @param request_token     Identifies a particular request, passed with the
 *                          #ICE_TSPlayerRequestCallback.
 * @param data_size         The amount of data provided as a multiple of
 *                          188 bytes.
 * @param end_of_stream     Indicates if there is any more data to request.
 *                          If frost_true, no more requests should be made.
 *
 * @return #frost_true if the data was received successfully, #frost_false
 * if the data cannot be handled, e.g. the player was stopped.
 */
frost_bool ICE_TSPlayerRequestComplete(ICE_TSPlayerHandle * handle,
                                       void * request_token,
                                       frost_uint32 data_size,
                                       frost_bool end_of_stream);

/**
 * @brief Flush any buffers used for transport stream playback.
 *
 * This function indicates that there will be a discontinuity in the incoming
 * requested data, such as from the user skipping backward or forwards.
 * Therefore, to ensure playback reflects the correct data, any previous data
 * received and buffered for demultiplexing or decoding should be invalidated.
 *
 * @param handle            Handle to the required TS player.
 */
void ICE_TSPlayerFlush(ICE_TSPlayerHandle * handle);

/**
 * @brief Stops streaming of a transport stream.
 *
 * This function stops the streaming started with ICE_TSPlayerStart() and
 * releases all resources allocated for it.  A callback must not be made as a
 * result of, or after, calling this function.
 *
 * @param handle            Handle to the required TS player.
 *                          Becomes invalid after a successful call to this
 *                          function and must not be used again.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TSPlayerStop(ICE_TSPlayerHandle * handle);

/**
 * @brief Controls whether streaming of a transport stream is enabled.
 *
 * Suspends or resumes streaming started with ICE_TSPlayerStart().
 * When pausing, the decoders must also freeze output as soon as possible and
 * they must continue smoothly from the same position when streaming is resumed.
 *
 * @param handle Handle to the required TS player.
 * @param pause  frost_true to pause the stream, frost_false to resume.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TSPlayerPause(ICE_TSPlayerHandle * handle, frost_bool pause);

/**
 * @brief Determines the current playback position.
 *
 * The playback position is a byte offset relative to data received since the
 * last call to ICE_TSPlayerStart() or ICE_TSPlayerFlush().
 *
 * The implementation should attempt to calculate, as accurately as possible,
 * the position corresponding to content currently being presented, such as
 * audio and/or video.  If this is not possible, or no content is defined for
 * presentation (e.g. ICE_VideoStart() and ICE_AudioStart() have not been
 * called), the the position may simply correspond to the number of bytes of
 * data received.
 *
 * @note The accuracy of the position affects operations based on this, such as
 *       skipping backward or forward and stopping at a predefined position.
 *
 * @note Freeview ICS requires that the reported position is accurate to
 *       within +/-5 seconds.
 *
 * @param handle Handle to the required TS player.
 * @param position The returned playback position in bytes.
 *
 * @return #frost_true on success and position is set, #frost_false on failure.
 */
frost_bool ICE_TSPlayerGetPosition(ICE_TSPlayerHandle * handle,
                                   frost_uint64 * position);
#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_TS_PLAYER_H_INCLUDED */
