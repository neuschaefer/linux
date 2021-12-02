/**
 * Copyright 2002 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for Audio
 *
 * This file defines the ICE interfaces to allow manipulation of audio
 * streams and samples.  All functions in this interface are
 * implemented in STB-specific code and called by Cabot code.
 */

#ifndef CABOT_ICE_ICE_AUDIO_H_INCLUDED
#define CABOT_ICE_ICE_AUDIO_H_INCLUDED

#include "ice/ice_audio_types.h"
#include "frost/frost_basictypes.h"
#include "ice/ice_device_number.h"
#include "ice/ice_av.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Initialise the Audio device.
 *
 * Initialise the required audio device and its connection to the DEMUX.
 *
 * @param params The initialisation parameters indicate the required
 *        audio device.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_AudioInit(const ICE_AudioInitParams* params);


/**
 * @brief Open a connection to the Audio device.
 *
 * Attempts to open a connection to the audio device identified and
 * return a handle for subsequent communication with the device.
 *
 * @param params The open parameters indicate the required audio device.
 *
 * @return Returns a pointer to the handle to the device on success,
 *         and a NULL pointer on failure.
 */
ICE_AudioHandle* ICE_AudioOpen(const ICE_AudioOpenParams* params);


/**
 * @brief Closes the connection to the audio device and releases any resources.
 *
 * @param handle The handle to the audio device to be closed.
 */
void ICE_AudioClose(ICE_AudioHandle* handle);

/**
 * @brief Configure the transport stream source for subsequent calls to 
 * ICE_AudioStart.
 *
 * @param handle The handle to the required audio device.
 * @param source The source of the transport stream, e.g. tuner 0, tuner 1 or 
 * hard disk.
 *
 * This function will only be called when the audio decoder is stopped.
 * 
 * @note For single tuner platforms with no HDD, this function may be ignored.
 *
 * @note This setting does not affect audio samples played via
 *       ICE_AudioFileLoad() and ICE_AudioPlaySample().
 */
void ICE_AudioSelectSource(ICE_AudioHandle* handle,
                           ICE_DeviceSource source);

/**
 * @brief Check whether the audio stream type is supported
 *
 * This function is called to determine if ICE is able to provide
 * support for the specified stream type. That is, if requested to
 * start decoding audio of this type (eg AAC) that the
 * platform has a suitable decoder.
 * It must be called before a call to ICE_AudioStart, as if this
 * function indicates that the given stream type is not supported
 * then it is expected that Eclipse will not call ICE_AudioStart.
 *
 * @param audio_stream_type Stream type to be checked.
 *
 * @return #frost_true on type supported, #frost_false not supported
 */
frost_bool ICE_IsAudioStreamTypeSupported(ICE_AudioStreamType audio_stream_type);

/**
 * @brief Start decoding audio stream specified by PID.
 *
 * Start decoding the input audio elementary stream specified by
 * <code>audio_pid</code>.  If another call is made to this function prior to
 * a call to ICE_AudioStop() the behaviour is undefined.
 *
 * Note that this function will be proceeded by a call to 
 * ICE_IsAudioStreamTypeSupported, to ensure that the platform is able to
 * decode the supplied audio stream. If this function is called with a
 * stream type that is not supported it will return frost_false, but
 * provide no further information on why the failure occurred.
 *
 * @param handle The handle to the required audio device.
 * @param audio_pid Audio PID to decode.
 * @param user_data Context to be maintained and passed back through
 *        callback function.
 * @param callback_func Function to call on first audio frame decode.
 * @param audio_stream_type The type of the audio stream (MPEG-2/AC3/AAC/HE-AAC)
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_AudioStart(ICE_AudioHandle* handle, frost_uint audio_pid,
                          void* user_data,
                          IceAudioDecodingCallbackFunction callback_func,
                          ICE_AudioStreamType audio_stream_type);


/**
 * @brief Stop playback of currently selected audio.
 *
 * Ceases playback of any currently selected audio.  This applies to
 * audio playing as a result of either a call to ICE_AudioStart() or
 * ICE_AudioPlaySample().  The implementation must release the audio
 * PID.  Calling ICE_AudioStop() when there is no active audio has no
 * effect and will return #frost_true.
 *
 * @param handle The handle to the required audio device.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_AudioStop(ICE_AudioHandle* handle);


/**
 * @brief Set the volume of the currently playing audio stream or
 * sample (if any).
 *
 * Sets the volume of the currently playing audio stream or sample
 * file (if any).  Eclipse implements audio-muting by a call to
 * ICE_AudioVolume() with <code>percent_volume_level</code> set to zero.
 *
 * @param handle The handle to the required audio device.
 * @param percent_volume_level New volume level as a percentage of
 *        the maximum volume.  Values over 100 will be treated as 100.
 * @param isMuted Definition of audio is muted or not. True used if audio is muted.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_AudioVolume(ICE_AudioHandle* handle,
                           frost_uint percent_volume_level, 
                           frost_bool isMuted);


/**
 * @brief Requests loading of an audio sample.
 *
 * Requests that the audio hardware should load an MPEG-1 audio sample in
 * order that it can subsequently be played with a call to
 * ICE_AudioPlaySample().  Only a single audio sample may be loaded at a time.
 *
 * The data pointer must not be retained by the ICE implementation after the
 * call to ICE_AudioFileLoad() returns, as Eclipse is free to re-use the
 * memory.  Therefore, the implementation must either make a copy of the data
 * or process it and ready it in AV memory.
 *
 * @note If the implementation uses resources to prepare the sample, these
 * resources may be released by the implementation when other audio calls are
 * made.  Thus, Eclipse does not rely on an sample still being loaded after
 * intervening audio calls are made, and instead has to reload the sample.
 *
 * @param handle The handle to the required audio device.
 * @param data Pointer to audio data.
 * @param sample_size Length of audio data.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_AudioFileLoad(ICE_AudioHandle* handle, const frost_uint8 *data,
                             frost_ulong sample_size);


/**
 * @brief Play sample previously loaded into memory.
 *
 * Requests that the audio hardware should play back the audio sample
 * that was loaded into memory by ICE_AudioFileLoad().
 *
 * @param handle The handle to the required audio device.
 * @param loop_count Number of times the sample should be played.  Special
 *        value 0 indicates that the sample should continue to loop
 *        until a subsequent call to ICE_AudioStop() is made.
 * @param user_data Context to be maintained and passed back through
 *        callback function.
 * @param callback_func Function to call on completion of the last loop.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_AudioPlaySample(ICE_AudioHandle* handle, frost_uint loop_count,
                               void* user_data,
                               IcePlayCompleteCallbackFunction callback_func);


/**
 * @brief Pause (or resume) playback of an audio sample.
 *
 * This function pauses (or resumes) the playing of an audio sample.  Resuming
 * playback of a paused sample file will continue playback from the point at
 * which the sample was paused.
 *
 * The behaviour of this function is undefined when applied to a live audio
 * stream.  To mute/unmute a live audio stream the ICE_AudioVolume() function
 * should be used.
 *
 * @param handle The handle to the required audio device.
 * @param freeze #ICE_freeze_on or #ICE_freeze_off.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_AudioPause(ICE_AudioHandle* handle, ICE_FreezeMode freeze);


/**
 * @brief Set the required audio channel output mode
 *
 * Sets the audio channel mode. This does not change which stream
 * is played, just how it is output.
 *
 * @param handle The handle to the required audio device.
 * @param audio_channel_mode The required #ICE_AudioChannelMode enumeration.
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note This setting does not affect audio samples played via
 *       ICE_AudioFileLoad() and ICE_AudioPlaySample().
 */
frost_bool ICE_SetAudioChannelMode(ICE_AudioHandle* handle,
                                   ICE_AudioChannelMode audio_channel_mode);

/**
 * @brief Gets the audio channel info for the currently playing audio stream.
 *
 * The MPEG audio stream carries information about the channels of audio in the stream.
 * E.g. some audio streams might be stereo and others might carry two separate mono channels.
 *
 * @param handle The handle to the required audio device.
 * @param audio_channel_info The audio channel information of the currently playing audio.
 *
 * @return #frost_true if the channel info could be got, otherwise #frost_false.
 */
frost_bool ICE_GetAudioChannelInfo(ICE_AudioHandle* handle,
                                   ICE_AudioChannelInfo* audio_channel_info);

/**
 * @brief This sets the type of audio that comes out of the Digital Audio Out connector, if any exist, e.g. S/PDIF Optical
 *
 * This is to allow external AV receivers to perform the audio decoding.
 *
 * @param audio_digital_out_type The type of audio (e.g. PCM/compressed).
 *
 * @note The default mode is PCM.
 * @see ICE_AudioDigitalOut
 *
 */
void ICE_AudioSetDigitalOut(ICE_AudioDigitalOut audio_digital_out_type);


void ICE_SetAudioStreamHeAACL2Status(frost_bool audio_strem_type_he_aac_L2);

void ICE_SetServiceTypeRadio(frost_bool is_radio_service); 


/**
 * @brief Set the volume of the currently playing audio stream or
 * sample (if any) on the Digital Audio Out connector, if any exist, e.g. S/PDIF Optical
 *
 * @param digital_output_level_dB New output level expressed as a decibel (dB) level
 *
 */
void ICE_AudioSetDigitalOutputLevel(frost_int digital_output_level_dB);

/**
 * @brief Register a callback that will notify Eclipse about audio status changes.
 * 
 * @param handle The handle to the required audio device.
 * @param callback The callback function that will be called 
 *        when audio status changes.
 * @param user_data Context to be maintained and passed back through
 *        callback function.
 * 
 * @note If USE_TERR_ANALOG_DIGITAL_TUNER is not enabled, then you do not
 *       need to implement this function.
 */
void ICE_RegisterAudioStatusCallback(ICE_AudioHandle* handle,
                                          ICE_AudioStatusCallback callback,
                                          void* user_data);

/**
 * @brief Play single frequency audio signal.
 *
 * Requests that the audio hardware should generate an audio beep
 *
 * @param handle The handle to the required audio device.
 * @param frequency Audio signal frequency (Hz)
 * @param duration Audio signal duration (ms)
 * @param percent_volume_level Volume attenuation
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note If INCLUDE_AUDIO_DESCRIPTION is not enabled
 *       this function need not be implemented.
 */
frost_bool ICE_AudioPlayBeep(ICE_AudioHandle* handle,
                             frost_uint frequency,
                             frost_uint duration,
                             frost_uint percent_volume_level);

/**
 * @brief Stop playing single frequency audio signal.
 *
 * Requests that the audio hardware should stop an audio beep and unmute TV if necessary
 *
 * @param handle The handle to the required audio device.
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note If INCLUDE_AUDIO_DESCRIPTION is not enabled
 *       this function need not be implemented.
 */
frost_bool ICE_AudioStopBeep(ICE_AudioHandle* handle);

/**
 * @brief Adjust the volume for audio sources relative to the ICE_AudioVolume 
 * percentage.
 *
 * @param adjustment The adjustment to be applied.
 *
 * The volume adjustment will range from -15 to +15. 0 means no adjustment.
 *
 * @note This adjustment should also be applied to the Audio Description volume.
 */

void ICE_AudioVolumeCorrection(frost_int8 adjustment);

/**
 * @brief to get current Audio Stream Type from Audio Decoder
 * 
 * @param handle The handle to the required audio device.
 *
 * @return ICE_AudioStreamType enum
 */

ICE_AudioStreamType ICE_GetAudioStreamType(ICE_AudioHandle* handle);

/**
 * @brief Set the audio delay of the currently playing audio stream or
 * sample (if any).
 * 
 * @param handle The handle to the required audio device.
 * @param audio_delay_level New audio delay level as time in ms.
 *        Values over 400 ms will be treated as 400 ms.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_AudioDelay(ICE_AudioHandle* handle,
                           frost_uint audio_delay_level);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_AUDIO_H_INCLUDED*/
