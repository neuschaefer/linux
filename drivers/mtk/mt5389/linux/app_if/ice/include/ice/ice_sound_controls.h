/**
 * Copyright 2007 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for analogue TV sound control data types and functions.
 *
 * This file defines the common ICE data structures and functions to allow
 * manipulation of analogue TV sound features.

 * @note Platforms that do not support analogue video inputs
 *       do not need to implement the functions in this interface.
 */

#ifndef CABOT_ICE_ICE_SOUND_CONTROLS_H_INCLUDED
#define CABOT_ICE_ICE_SOUND_CONTROLS_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_audio_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Type definition for sound profiles, these are used to set the audio equalizer.
 */
typedef enum ICE_SoundProfile
{
    ice_sound_profile_music = 0,
    ice_sound_profile_speech,
    /* Add new Profiles before this line. */
    ice_sound_profile_user,
    ice_sound_profile_count   /** Number of different sound Profiles. */
} ICE_SoundProfile;


/**
 * @brief Type definition for wide sound modes.
 */
typedef enum ICE_WideSound
{
    ice_wide_sound_off,
    ice_wide_sound_mono,
    ice_wide_sound_stereo,
    ice_wide_sound_invalid
}ICE_WideSound;


/**
 * @brief Type definition for sound modes.
 */
typedef enum ICE_SoundMode
{
    ice_sound_mode_fm_mono,
    ice_sound_mode_nicam_mono,
    ice_sound_mode_nicam_stereo,
    ice_sound_german_mono,
    ice_sound_german_stereo,
    ice_sound_mode_german_dual_I,
    ice_sound_mode_german_dual_II,
    ice_sound_mode_nicam_dual_I,
    ice_sound_mode_nicam_dual_II,
    ice_sound_mode_invalid
}ICE_SoundMode;

/**
 * @brief Available sound modes
 */
typedef enum ICE_AvailableSoundMode
{
    ice_available_sound_mode_none = 0,
    ice_available_sound_mode_fm_mono = 0x0001,
    ice_available_sound_mode_nicam_mono = 0x0002,
    ice_available_sound_mode_nicam_stereo = 0x0004,
    ice_available_sound_mode_german_mono = 0x0008,
    ice_available_sound_mode_german_stereo = 0x0010,
    ice_available_sound_mode_german_dual = 0x0020,
    ice_available_sound_mode_nicam_dual = 0x0040
}ICE_AvailableSoundMode;


/**
 * @brief Sound features that may be supported by platform.
 */
typedef enum ICE_SoundFeatures
{
    ice_sound_feature_none                    = 0x00000,
    ice_sound_feature_dynamic_bass            = 0x00001,  /** ICE_SoundControlEnableDynamicBass */
    ice_sound_feature_srs                     = 0x00002,  /** ICE_SoundControlSRS */
    ice_sound_feature_surround_mode           = 0x00004,  /** ICE_SoundControlEnableSurroundMode */
    ice_sound_feature_subwoofer               = 0x00008,  /** ICE_SoundControlEnableSubwoofer */
    ice_sound_feature_avl                     = 0x00010,  /** ICE_SoundControlEnableAVL */
    ice_sound_feature_equalizer               = 0x00020,
    ice_sound_feature_digital_out             = 0x00040,
    ice_sound_feature_ac3                     = 0x00080,  /** Internally changes the audio pid used (PCM or AC3). This is because on ST chips there is only one audio decoder. */
    ice_sound_feature_spdif_out               = 0x00100,  /** ICE_SoundControlEnableSPDIFOut */
    ice_sound_feature_effect                  = 0x00200,  /** ICE_SoundControlEnableEffect */
    ice_sound_feature_headphones_volume_bar   = 0x00400,  /** ICE_SoundControlSetHeadphoneVolume */
    ice_sound_feature_headphones_equalizer    = 0x00800,
    ice_sound_feature_loudness                = 0x01000   /** ICE_SoundControlEnableLoudness */
}ICE_SoundFeatures;


/**
 * @brief Type definition for parametric equalizer .
 */
typedef struct ICE_ParametricEqualizerParams
{
    frost_uint32 band;
    frost_uint32 Foh;
    frost_uint32 Fol;
    frost_uint32 Gain;
    frost_uint32 Q;
}ICE_ParametricEqualizerParams;


/**
 * @brief Equalizer features that may be supported by platform.
 */
typedef struct ICE_EqualizerFeatures
{
    /**
     *  Number of equalizer bands that platform  provides
     */
    frost_uint8 uiNbrEqualizerBand;

    /**
     * Equalizer bands name (100Hz,330Hz...) that will be shown by UI
     */
    const char** cppChannelNames;   /* Bands Name (100Hz,330Hz...) */

}ICE_EqualizerFeatures;

/**
 * @brief Set of Available sound features on the platform.
 */
typedef frost_uint ICE_AvailableSoundFeatures;


/**
 * @brief Get available sound modes
 *
 * @param handle The handle of the audio device for which the sound modes will be returned.
 *
 * @return The available sound modes as a bitfield of ICE_AvailableSoundMode values
 */
frost_uint32 ICE_SoundControlGetAvailableSoundModes(ICE_AudioHandle* handle);


/**
 * @brief Get the minimum and maximum values values for balance.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param min_value Minimum balance value that will be filled by this function.
 * @param max_value Maximum balance value that will be filled by this function.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlGetBalanceRange(ICE_AudioHandle* handle,
                                                       frost_int* min_value, frost_int* max_value);

/**
 * @brief Set the balance to the specified value.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param balance_level The balance value that will be applied.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlSetBalance(ICE_AudioHandle* handle, frost_int balance_level);


/**
 * @brief Set the headphone balance to the specified value.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param balance_level The balance value that will be applied.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlSetHeadphoneBalance(ICE_AudioHandle* handle, frost_int balance_level);

/**
 * @brief Get the minimum and maximum values values for headphone bass.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param min_value Minimum headphone bass value that will be filled by this function.
 * @param max_value Maximum headphone bass value that will be filled by this function.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlGetHeadphoneBassRange(ICE_AudioHandle* handle,
                                                       frost_int* min_value, frost_int* max_value);

/**
 * @brief Set the headphone bass treble to the specified value.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param bass_level The bass value that will be applied.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlSetHeadphoneBass(ICE_AudioHandle* handle, frost_int bass_level);

/**
 * @brief Get the minimum and maximum values values for headphone treble.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param min_value Minimum headphone treble value that will be filled by this function.
 * @param max_value Maximum headphone treble value that will be filled by this function.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlGetHeadphoneTrebleRange(ICE_AudioHandle* handle,
                                                       frost_int* min_value, frost_int* max_value);

/**
 * @brief Set the headphone treble to the specified value.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param treble_level The treble value that will be applied.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlSetHeadphoneTreble(ICE_AudioHandle* handle, frost_int treble_level);

/**
 * @brief Set the headphone mute value.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param muted  When Headphone is muted, value will be true, else false
 *
 */
void ICE_SoundControlSetHeadphoneMute(ICE_AudioHandle* handle, frost_bool muted);

/**
 * @brief Set the headphone volume of the currently playing audio stream or
 * sample (if any).
 *
 * Sets the headphone  volume of the currently playing audio stream or sample
 * file (if any).  Eclipse implements audio-muting by a call to
 * ICE_SoundControlSetHeadphoneVolume() with <code>percent_volume_level</code> set to zero.
 *
 * @param handle The handle to the required audio device.
 * @param volume_level New volume level as a percentage of
 *        the maximum volume.  Values over 100 will be treated as 100.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlSetHeadphoneVolume(ICE_AudioHandle* handle, frost_int volume_level);


/**
 * @brief Set the sound mode to the specified value.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param sound_mode The sound mode that will be applied.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlSetSoundMode(ICE_AudioHandle* handle, ICE_SoundMode sound_mode);


/**
 * @brief Get the current sound mode.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 *
 * @return Current sound mode.
 */
ICE_SoundMode ICE_SoundControlGetSoundMode(ICE_AudioHandle* handle);


/**
 * @brief Enables / Disables the automatic volume levelling feature.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param enable If #frost_true, enable AVL; if #frost_false, disable AVL.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlEnableAVL(ICE_AudioHandle* handle, frost_bool enable);

/**
 * @brief Enables / Disables Lineout option from headphone jack.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param enable If #frost_true, enable lineout from headphone; if #frost_false, disable lineout from headphone.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlSetLineoutFromHeadphone(ICE_AudioHandle* handle, frost_bool enable);

/**
 * @brief Returns Lineout option from headphone jack.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlGetLineoutFromHeadphone(ICE_AudioHandle* handle);

/**
 * @brief Enables / Disables sound effects.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param enable If #frost_true, enable effects; if #frost_false, disable effects.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlEnableEffect(ICE_AudioHandle* handle, frost_bool enable);

/**
 * @brief set dynamic bass gain.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param bass_level in range -6db/+6db.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlSetBassGain(ICE_AudioHandle* handle, frost_int bass_level);

/**
 * @brief Enables / Disables dynamic bass feature.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param enable If #frost_true, enable dynamic bass; if #frost_false, disable dynamic bass.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlEnableDynamicBass(ICE_AudioHandle* handle, frost_bool enable);

/**
 * @brief Sets the wide sound mode.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param wide_sound The wide sound mode that will be applied.
 *
 * Possible vide sound vaules are ice_wide_sound_off, ice_wide_sound_mono, ice_wide_sound_stereo.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlSetWideSound(ICE_AudioHandle* handle, ICE_WideSound wide_sound);


/**
 * @brief Enables / Disables Surround Mode feature.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param enable If #frost_true, enable surround mode ; if #frost_false, disable it.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlEnableSurroundMode(ICE_AudioHandle* handle, frost_bool enable);


/**
 * @brief Enables / Disables SRS feature.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param enable If #frost_true, enable SRS; if #frost_false, disable SRS.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlEnableSRS(ICE_AudioHandle* handle, frost_bool enable);


/**
 * @brief Enables / Disables subwoofer.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param enable If #frost_true, enable subwoofer; if #frost_false, disable subwoofer.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlEnableSubwoofer(ICE_AudioHandle* handle, frost_bool enable);

/**
 * @brief Enables / Disables SPDIF output.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param enable If #frost_true, enable SPDIF output; if #frost_false, disable SPDIF output.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlEnableSPDIFOut(ICE_AudioHandle* handle, frost_bool enable);

/**
 * @brief Enables / Disables Loudness.
 *
 * @param handle The handle for the audio device to which settings will be applied.
 * @param enable If #frost_true, enable loudness; if #frost_false, disable loudness.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SoundControlEnableLoudness(ICE_AudioHandle* handle, frost_bool enable);


/**
 * @brief Get available features for sound.
 *
 * @return Available sound features.
 */
ICE_AvailableSoundFeatures ICE_SoundControlGetAvailableFeatures(void);

/**
 * @brief Get available features of equalizer.
 *
 * @param param Equalizer properties (How many equalizer band exist, what are their names(100Hz,1KHz...), etc.)
 */
void ICE_SoundControlGetEqualizerFeatures(ICE_EqualizerFeatures* param);

/**
 * @brief Set equalizer band.
 *
 * @param band Which equalizer band will be set.
 *
 * @param value Value will be set to the band that specified with Band parameter.
 *
 * @note  like picture quality the audio quality is set after the AV source has been selected.
 *
 * @return frost_true on success, frost_false otherwise.
 */
frost_bool ICE_SoundControlSetEqualizerBand(frost_uint8 band, frost_int8 value);

/**
 * @brief Get equalizer band .
 *
 * @param band Which equalizer band this range applies to.
 *
 * @param min,max The minimum and minimum values the user can use select
 *                when calling <code>ICE_SoundControlSetEqualizerBand</code>.
 */
void ICE_SoundControlGetEqualizerRange(frost_uint8 band, frost_int *min, frost_int *max);

/**
 * @brief Get equalizer band .
 *
 * @param source Which av source does this setting applies to.
 * @param mode   Which mode does this setting applies to.
 * @param band   Which equalizer band this setting applies to.
 *
 * @return frost_true on success, frost_false otherwise.
 */
frost_int8 ICE_SoundControlGetEqualizerDefaultValue(frost_uint source, ICE_SoundProfile mode, frost_uint8 band);

/**
 * @brief Set parametric equalizer values.
 *
 * param  params Parametric equalizer parameters  which will be set.
 *
 * @return frost_true on success, frost_false otherwise.
 */
frost_bool ICE_SoundControlSetParametricEqualizer(ICE_ParametricEqualizerParams* params);


/**
 * @brief Mute Monitor audio.
 * 
 * param muted true if mute ON monitor audio; false if mute OFF monitor audio. 
 */
void ICE_SoundControlMuteMonitor( frost_bool muted);

/**
 * @brief Set the speaker settings for the platform
 *
 * @param isCloseToWall - frost_true if the TV/speakers are close to a wall, frost_false otherwise
 *
 * @return frost_true on success, frost_false otherwise.
 */
frost_bool ICE_SoundControlIsSpeakerCloseToWall(frost_bool isCloseToWall);

/**
 * @brief Informs about heaphone connection status
 *
 *
 * Returns TRUE if headphone is connected otherwise returns FALSE
 *
 */

frost_bool ICE_SoundControlIsHeadphoneConnected(void);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_SOUND_CONTROLS_H_INCLUDED*/

