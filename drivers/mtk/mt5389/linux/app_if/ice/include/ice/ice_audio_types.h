/**
 * Copyright 2006 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for Audio types
 *
 * This file defines the ICE structures used by the ice_audio interface
 *
 */

#ifndef CABOT_ICE_ICE_AUDIO_TYPES_H_INCLUDED
#define CABOT_ICE_ICE_AUDIO_TYPES_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_device_number.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque type used to identify an open connection.
 */
typedef struct ICE_AudioHandle ICE_AudioHandle;


/**
 * @brief Audio device Initialisation parameters.
 */
typedef struct ICE_AudioInitParams
{
    /**
     * Identifies the instance of the Audio device.
     */
    ICE_DeviceNumber device_number;

} ICE_AudioInitParams;


/**
 * @brief Possible audio channel output modes.
 */
typedef enum ICE_AudioChannelMode
{
    /**
     * Mono output.
     */
    ICE_audio_channel_mode_mono,

    /**
     * Stereo output.
     */
    ICE_audio_channel_mode_stereo,

    /**
     * Output of the left channel of a dual-mono stream.
     */
    ICE_audio_channel_mode_dual_mono_left,

    /**
     * Output of the right channel of a dual-mono stream.
     */
    ICE_audio_channel_mode_dual_mono_right

} ICE_AudioChannelMode;


/**
 * @brief Possible audio stream types.
 */
typedef enum ICE_AudioStreamType
{
    /**
     * MPEG audio.
     */
    ICE_audio_stream_type_mpeg,

    /**
     * AC-3 audio.
     */
    ICE_audio_stream_type_ac3,

    /**
     * EAC-3 audio.
     */
    ICE_audio_stream_type_eac3,
    
    /**
     * AAC audio.
     */
    ICE_audio_stream_type_aac,

    /**
     * HE-AAC audio.
     */
    ICE_audio_stream_type_he_aac,

    /**
     * HE-AAC ADTS type audio.
     */
     ICE_audio_stream_type_he_aac_adts,

    /**
    * PCM audio
    */
    ICE_audio_stream_type_pcm        

} ICE_AudioStreamType;


/**
 * @brief The channel info for the current audio.
 */
typedef enum ICE_AudioChannelInfo
{
    /**
     * A single mono channel source.
     */
    ICE_audio_channel_info_single_mono,

    /**
     * A dual-mono channel source (i.e. different left and right sources)
     */
    ICE_audio_channel_info_dual_mono,

    /**
     * A stereo source (2 channels).
     */
    ICE_audio_channel_info_stereo,

    ICE_audio_channel_info_undefined    

} ICE_AudioChannelInfo;

/**
 * @brief Digital audio output modes.
 */
typedef enum ICE_AudioDigitalOut
{
    /**
     * PCM audio.
     */
    ICE_audio_digital_out_pcm,

    /**
     * Compressed audio
     * (This is anything other than PCM.
     *  Could be AC3 [aka DD], E-AC3 [aka DD+], AC3, AC3-HE etc)
     */
    ICE_audio_digital_out_compressed

} ICE_AudioDigitalOut;

/**
* @brief Definiton of audio speaker out type
*/
typedef enum ICE_AudioSpeakerOut
{
    /**
     * Main speaker left.
     */
    ICE_audio_speaker_main_left,

    /**
     * Main speaker right.
     */
    ICE_audio_speaker_main_right,

    /**
     * Other speakers.
     */
    ICE_audio_speaker_one,
    ICE_audio_speaker_two,
    ICE_audio_speaker_three,
    ICE_audio_speaker_four,
    ICE_audio_speaker_five,
    ICE_audio_speaker_sub_woofer,
    ICE_audio_speaker_max
} ICE_AudioSpeakerOut;

/**
 * @brief Audio device open parameters.
 */
typedef struct ICE_AudioOpenParams
{
    /**
     * Identifies the instance of the Audio device.
     */
    ICE_DeviceNumber device_number;

    /**
     * The initial audio signal output mode
     */
    ICE_AudioChannelMode audio_mode;

} ICE_AudioOpenParams;

/**
 * @brief Possible audio status events.
 */
typedef enum ICE_AudioStatus 
{
    /**
     * Send when digital audio decoding starts after a call to ICE_AudioStart().
     *
     * The ICE_AudioStatusDigitalAudioDecodingError should be sent when decoding
     * becomes impossible, either after a call to ICE_VideoStart() or during operation.
     */
    ICE_AudioStatusDigitalAudioReady,

    /**
     * Send when audio decoding stops or doesn't start after a call to ICE_VideoStart().
     * This may indicate encrypted stream.
     *
     * The ICE_AudioStatusDigitalAudioReady should be sent when audio becomes 
     * decodable (again).
     */
    ICE_AudioStatusDigitalAudioDecodingError,

    /**
     * Send when analog audio decoding starts after a call to ICE_AudioStart().
     */
    ICE_AudioStatusAnalogAudioReady,

    /**
     * Send when audio from external sources is ready to be unmuted after a call to ICE_AudioStart
     */
    ICE_AudioStatusExternalAudioReady,
    
    /**
     * Send when sound system change is detected for analog audio.
     */
    ICE_AudioStatusAnalogSoundSystemChanged,

    /**
     * Send when sound system change is detected for digital audio.
     */
    ICE_AudioStatusDigitalSoundSystemChanged

} ICE_AudioStatus;

/**
 * @brief Callback function to raise a decoding event.
 * 
 * This callback function should be called when the first
 * audio frame is decoded, when the audio channel mode changes,
 * or when audio decoding cannot continue.
 * 
 * If the platform cannot detect when the first frame is decoded 
 * or when the audio channel mode changes, then it should ensure
 * a configurable delay between starting audio decoding and playing
 * audio.
 *
 * This callback signals a semaphore.
 *
 * @param user_data Data to be passed back when the callback is
 *        raised.  The user_data is given to ICE_AudioStart().
 *
 * @param status New decoding status, either "ready" or "cannot decode".
 */
typedef void (*IceAudioDecodingCallbackFunction)(void* user_data, ICE_AudioStatus status);


/**
 * @brief Callback function to let the caller know when an audio clip
 *        has completed playing.
 *
 * The supplied callback function should be called when the audio sample
 * played in the call to ICE_AudioPlaySample() has completed all its required
 * loops.
 *
 * This callback signals a semaphore.
 *
 * @param user_data Data to be passed back when the callback is
 *        raised.  The user_data is given to ICE_AudioPlaySample().
 */
typedef void (*IcePlayCompleteCallbackFunction)(void* user_data);

/**
 * @brief Callback function to inform Eclipse of the audio decoder status.
 *
 * See the comments in ICE_AudioStatus for when this callback should be
 * called.
 *
 * @param user_data As supplied in the call to ICE_AudioOpen().
 * @param status The audio decoder status.
 */
typedef void (*ICE_AudioStatusCallback)(void* user_data, ICE_AudioStatus status);


#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_AUDIO_TYPES_H_INCLUDED*/
