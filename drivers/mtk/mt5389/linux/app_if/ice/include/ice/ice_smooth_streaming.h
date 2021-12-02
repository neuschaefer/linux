/**
 * @file ice_smooth_streaming.h
 *
 * @brief This file holds the general high level ICE declarations, definitions
 *        and function prototypes neccessary for Smooth Streaming implementation.
 *
 * @note Copyright (c) 2011 Vestel
 */

#ifndef ICE_ICE_SMOOTH_STREAMING_H_INCLUDED
#define ICE_ICE_SMOOTH_STREAMING_H_INCLUDED

#include "config/ice_product_mstar/sspk_1111_source_code_is_used_for_library_creation.h"

#include "frost/frost_basictypes.h"


/***************************************************************************/
/**
 * @name Type Definitions
 * @{
 */
/***************************************************************************/

/**
 * @brief Opaque type used to identify an open connection.
 */
typedef struct ICE_SmoothStreamingHandle ICE_SmoothStreamingHandle;

typedef enum ICE_SmoothStreamingStatus
{
    /**
     * Send when MediaTransportTunerState_Playing is received as TunerState status update.
     */
    ice_smooth_streaming_started,

    /**
     * Send when MediaTransportTunerState_Completed is received as TunerState status update.
     */
    ice_smooth_streaming_completed,

    /**
     * Send when any kind of error is received.
     */
    ice_smooth_streaming_error
} ICE_SmoothStreamingStatus;

/**
 * @brief Callback function to inform MediaPlayer about the smooth streaming media status.
 *
 * See the comments in ICE_SmoothStreamingStatus for when this callback should be called.
 *
 * @param user_data As supplied in the call to ICE_SmoothStreamingOpen().
 *
 * @param status The smooth streaming status.
 */
typedef void (*ICE_SmoothStreamingStatusCallback)(void* user_data, ICE_SmoothStreamingStatus status);

/**
 * @brief Type definition for possible actions that can be checked for availability.
 */
typedef enum ICE_SmoothStreamingActionType
{
    ice_smooth_streaming_play,
    ice_smooth_streaming_pause,
    ice_smooth_streaming_stop,
    ice_smooth_streaming_fast_forward,
    ice_smooth_streaming_rewind,
    ice_smooth_streaming_skip,
    ice_smooth_streaming_seek,
    ice_smooth_streaming_scan,
    ice_smooth_streaming_replay
} ICE_SmoothStreamingActionType;

/**
 * @brief Possible commands that can be processed by ICE_SmoothStreamingProcessCommand function.
 */
typedef enum ICE_SmoothStreamingCommand
{
    ice_smooth_streaming_play_command,
    ice_smooth_streaming_pause_command,
    ice_smooth_streaming_modify_speed_command,
#ifndef SSPK_1111_SOURCE_CODE_IS_USED_FOR_LIBRARY_CREATION
    ice_smooth_streaming_step_command,
#endif
    ice_smooth_streaming_seek_command,
    ice_smooth_streaming_skip_command
} ICE_SmoothStreamingCommand;

#define SMOOTH_STREAMING_INVALID_COMMAND_PARAMETER (-1)

#define INCLUDE_SMOOTH_STREAMING    ICE_SmoothStreamingEnabled()

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************/
/**
 * @}
 * @name Smooth Streaming Functions
 * @{
 */
/***************************************************************************/

/**
 * @brief Initialise the Smooth Streaming object.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SmoothStreamingInit();

/**
 * @brief Terminate the Smooth Streaming object.
 */
void ICE_SmoothStreamingTerminate();

/**
 * @brief Open a connection to the smooth streaming media with given URL.
 *        Return a handle for subsequent communications about the content.
 *
 * @param url Address of the smooth streaming content manifest.
 *
 * @return Returns a pointer to the smooth streaming handle for manifest at
 *        given URL on success, and a NULL pointer on failure.
 */
ICE_SmoothStreamingHandle* ICE_SmoothStreamingOpen(const char* url,
                                                    ICE_SmoothStreamingStatusCallback callback_function,
                                                    void* user_data);

/**
 * @brief Close the connection of given handle, release related resources.
 *
 * @param handle The handle of content to be closed.
 */
void ICE_SmoothStreamingClose(ICE_SmoothStreamingHandle* handle);

/**
 * @brief Get the smooth streaming media URL of given handle.
 *
 * @param handle The handle of media to be queried for URL.
 *
 * @return Returns a character pointer to URL of content at given handle,
 *         NULL pointer on failure.
 */
const char* ICE_SmoothStreamingGetUrl(ICE_SmoothStreamingHandle* handle);

/**
 * @brief Check if given action is allowed.
 *
 * @param handle The handle of content to be checked.
 *
 * @param action_type Action type to be checked for availability.
 *
 * @return #frost_true if action is allowed, #frost_false otherwise.
 */
frost_bool ICE_SmoothStreamingIsActionAllowed(ICE_SmoothStreamingHandle* handle,
                                                ICE_SmoothStreamingActionType action_type);

/**
 * @brief Complete necessary processses for smooth streaming commands.
 *
 * @param handle The handle of content to be processed.
 *
 * @param command Smooth streaming command to be processed.
 *
 * @param command_parameter Parameter to be used while processing the command.
 *                          Only some of the commands need this parameter.
 *                          Its default value is defined by SMOOTH_STREAMING_INVALID_COMMAND_PARAMETER.
.*
 * @note Available commands and their usage:
 *      ice_smooth_streaming_play_command         : Plays the content with normal speed. command_parameter is needed.
 *      ice_smooth_streaming_pause_command        : Pauses the content. command_parameter is not needed.
 *      ice_smooth_streaming_modify_speed_command : Modifies play speed of content. command_parameter is used as playback
 *                                                  speed in per-thousandths. Eg. 2000 is double speed, 500 is half speed,
 *                                                  pause is zero, -3000 is triple speed in reverse.
 *      ice_smooth_streaming_step_command         : Moves the content one frame ahead. command_parameter is not needed.
 *                                                  Not applicable at SSPK V1111.
 *      ice_smooth_streaming_seek_command         : Seeks to the specified time of the content. command_parameter is used
 *                                                  as specified time which is the time difference form the start of content
 *                                                  in seconds.
 *      ice_smooth_streaming_skip_command         : Skips forward or backward in the content by specified duration.
 *                                                  command_parameter is used as seconds to skip (+ = forward, - = backward).
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SmoothStreamingProcessCommand(ICE_SmoothStreamingHandle* handle,
                                                ICE_SmoothStreamingCommand command,
                                                frost_int32 command_parameter );

/**
 * @brief Get the current playback time of media in seconds.
 *
 * @param handle The handle of content to be checked.
 *
 * @return Current playback time in seconds.
 */
frost_uint32 ICE_SmoothStreamingGetPosition(ICE_SmoothStreamingHandle* handle);

/**
 * @brief Gets the smooth streaming enabled
 *
 * @return current status
 */
frost_bool ICE_SmoothStreamingEnabled(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* ICE_ICE_SMOOTH_STREAMING_H_INCLUDED */
