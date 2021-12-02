/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief ICE interface for transport stream playback types.
 *
 * This file defines the ICE structures used by the ice_ts_player interface.
 */

#ifndef CABOT_ICE_ICE_TS_PLAYER_TYPES_H_INCLUDED
#define CABOT_ICE_ICE_TS_PLAYER_TYPES_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque type used to identify an open TS player.
 */
typedef struct ICE_TSPlayerHandle ICE_TSPlayerHandle;

/**
 * Callback function to inform Eclipse that more data is required.
 *
 * Eclipse will attempt to fill the given buffer with the requested amount of
 * TS data, consisting of multiples of complete 188-byte packets.
 * If there is not enough data to satisfy the request, e.g. the end of the TS
 * has been reached, then only the available amount will be filled
 * (which may be zero).
 *
 * Once an attempt to fill the buffer has been made, a request will always
 * result in a corresponding call to #ICE_TSPlayerRequestComplete().
 *
 * This function will not block significantly, and
 * #ICE_TSPlayerRequestComplete() will not be called from the same thread
 * context as the callback.
 *
 * @param request_token Must be passed to #ICE_TSPlayerRequestComplete() when
 *                      the request can be satisfied.
 * @param buffer To be filled with TS data.
 * @param buffer_size The amount of TS data required.
 * @param user_data The user_data parameter passed to ICE_TSPlayerStart().
 *
 * @return #frost_true if Eclipse will send some data, or #frost_false
 * if Eclipse is not able to send data at this time.
 */
typedef frost_bool (*ICE_TSPlayerRequestCallback)(void * request_token,
                                                  frost_uint8 * buffer,
                                                  frost_uint32 buffer_size,
                                                  void * user_data);

/**
 * Enumeration of possible TS player events passed to the
 * ICE_TSPlayerStatusCallback.
 */
typedef enum ICE_TSPlayerEvent
{
    /**
     * The player has started processing the transport stream, or at least
     * it is ready to accept requests for sections or PES data.
     * Note that requests for data may have been made prior to this event
     * being generated.
     */
    ice_ts_player_event_started,

    /**
     * The last of the media from the stream has been presented as a result
     * of end_of_stream being passed to #ICE_TSPlayerRequestComplete.
     */
    ice_ts_player_event_end_of_stream,

    /**
     * The player has been stalled due to a lack of incoming data.
     * This may be caused by requests not being satisfied quickly enough to
     * ensure smooth playback.
     * Streaming will continue if more data is received, at which time an
     * #ice_ts_player_event_resumed event will occur.
     * This event must not be sent at end of stream, instead send
     * #ice_ts_player_event_end_of_stream.
     */
    ice_ts_player_event_underflow,

    /**
     * The player has resumed after an #ice_ts_player_event_underflow event.
     */
    ice_ts_player_event_resumed,

    /**
     * The player has encountered non-transport stream format data and cannot
     * process it.
     *
     * #ICE_TSPlayerStop() must be called to release resources.
     */
    ice_ts_player_event_format_error,

    /**
     * The player has encountered an irrecoverable error streaming the
     * transport stream.
     *
     * #ICE_TSPlayerStop() must be called to release resources.
     */
    ice_ts_player_event_error

} ICE_TSPlayerEvent;

/**
 * Callback function to inform Eclipse of a TS player event.
 *
 * @param event         The event generated.
 * @param user_data     The user_data parameter passed to ICE_TSPlayerStart().
 */
typedef void (*ICE_TSPlayerStatusCallback)(ICE_TSPlayerEvent event,
                                           void * user_data);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_TS_PLAYER_TYPES_H_INCLUDED */
