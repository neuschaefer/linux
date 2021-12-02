/**
 * Copyright 2007 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief ICE interface for MP3 file playback.
 */ 

#ifndef CABOT_ICE_ICE_MP3_H_INCLUDED
#define CABOT_ICE_ICE_MP3_H_INCLUDED

#include "ice/ice_audio_types.h"
#include "frost/frost_basictypes.h"
#include "ice/ice_av.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Callback function to let the caller know when an MP3 file
 *        has completed playing.
 *
 * The supplied callback function should be called when the MP3 file
 * played in the call to #ICE_MP3FilePlay has completed.
 *
 * This callback signals a semaphore.
 *
 * @param user_data Data to be passed back when the callback is
 *        raised.  The user_data is given to #ICE_MP3FilePlay.
 */
typedef void (*ICE_MP3PlayCompleteCallbackFunction)(void* user_data);

/**
 * @brief Play an MP3 file.
 *
 * Requests that the audio hardware should play back the MP3 file
 * that is specified by the passed file specification.
 *
 * @param handle The handle to the required audio device.
 * @param filespec An absolute drive, partition, pathname and filename 
 *        specifier for the file.
 * @param user_data Context to be maintained and passed back through
 *        callback function.
 * @param callback_func Function to call on completion of playback.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_MP3FilePlay(ICE_AudioHandle* handle,
                             char const *filespec,
                             void* user_data,
                             IcePlayCompleteCallbackFunction callback_func);

/**
 * @brief Pause (or resume) playback of an MP3 file.
 *
 * This function pauses (or resumes) the playing of an MP3 file.  Resuming
 * playback of a paused MP3 file will continue playback from the point at
 * which the sample was paused.
 *
 * @param handle The handle to the required audio device.
 * @param freeze #ICE_freeze_on or #ICE_freeze_off.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_MP3FilePause(ICE_AudioHandle* handle, ICE_FreezeMode freeze);

/**
 * @brief Stop playback of an MP3 file.
 *
 * This function stops the playing of an MP3 file. Calling ICE_MP3FileStop()
 * when there is no playing MP3 file has no effect and will 
 * return #frost_true.
 *
 * @param handle The handle to the required audio device.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_MP3FileStop(ICE_AudioHandle* handle);

/**
 * @brief Skip a number of seconds of an MP3 file.
 *
 * A positive number of seconds implies a skip forward, while a negative number
 * implies a backward skip. Playback should not be affected when the passed
 * number of seconds is zero.
 *
 * In case the beginning of the file is reached playback should restart from 
 * the beginning. If the end of the file is reached playback should stop.
 *
 * Calling ICE_MP3FileSkipSeconds() when there is no playing MP3 file has no
 * effect and will return #frost_true.
 *
 * @param handle The handle to the required audio device.
 * @param seconds Number of seconds to skip forward or backward.
 * @param new_pos_seconds Variable to store new position as number of seconds from the start.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_MP3FileSkipSeconds(ICE_AudioHandle* handle, frost_int32 seconds, frost_uint32 *new_pos_seconds);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_MP3_H_INCLUDED */
