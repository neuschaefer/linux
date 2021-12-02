/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface to Sony/Philips Digital Interconnect Format S/PDIF
 *
 * See IEC 60958
 *
 * @note Platforms that do not support S/PDIF output do not need to implement 
 * the functions in this interface.
 */

#ifndef CABOT_ICE_ICE_SPDIF_H_INCLUDED
#define CABOT_ICE_ICE_SPDIF_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_audio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enables / Disables S/PDIF output.
 *
 * @param handle The handle for the audio device to which settings will be 
 *               applied.
 * @param enable If #frost_true, enable S/PDIF output; if #frost_false, disable 
 *               S/PDIF output. 
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SPDIFEnable(ICE_AudioHandle* handle, 
                           frost_bool enable);


/**
 * @brief This sets the type of audio that comes out of the SPDIF Out connector, if any exist.
 *
 * This is to allow external AV receivers to perform the audio decoding.
 *
 * @param audio_digital_out_type The type of audio (e.g. PCM/compressed).
 *
 * @note The default mode is PCM.
 * @see ICE_AudioDigitalOut
 *
 */
void ICE_SPDIFSetOutputType(ICE_AudioDigitalOut audio_digital_out_type);


/**
 * @brief Set the volume of the currently playing audio stream or
 * sample (if any) on the SPDIF Out connector, if any exist.
 *
 * @param digital_output_level_dB New output level expressed as a decibel (dB) level
 *
 */
void ICE_SPDIFSetOutputLevel(frost_int digital_output_level_dB);

/**
 * @brief Get the maximum supported S/PDIF output audio delay in milliseconds.
 *
 * It is recommended that a delay of 250 milliseconds or greater is supported.
 *
 * @param handle The handle for the audio device to get the maximum delay for.
 *
 * @return the maximum audio delay supported in milliseconds. If no audio delay
 * is supported then 0 must be returned. Should not be negative.
 */
frost_int32 ICE_SPDIFGetMaximumAudioDelay(ICE_AudioHandle* handle);
                   
/**
 * @brief Get the default S/PDIF output audio delay in milliseconds.
 *
 * This delay is typically set by the manufacturer to give a good audio/video 
 * synchronisation at first time installation. This may be required because a 
 * delay of 0 milliseconds would result in the audio being played out before 
 * the video, for example.
 *
 * @param handle The handle for the audio device to get the default delay for.
 *
 * @return The default audio delay in milliseconds. If no audio delay is 
 * supported than 0 must be returned. Should not be negative.
 */
frost_int32 ICE_SPDIFGetDefaultAudioDelay(ICE_AudioHandle* handle);

/**
 * @brief Get the current S/PDIF output audio delay in milliseconds.
 *
 * @param handle The handle for the audio device to get the delay for. 
 *
 * @return The current audio delay in milliseconds. This should not be negative.
 */
frost_int32 ICE_SPDIFGetAudioDelay(ICE_AudioHandle* handle);

/**
 * @brief Set S/PDIF output audio delay in milliseconds.
 *
 * A delay of 0 milliseconds is used as the minimum possible delay the hardware 
 * is capable of. This may result in the audio being played out before the 
 * video, this is intentional.
 *
 * @param handle The handle for the audio device to which the audio delay will 
 * be applied.
 *
 * @param delay The audio delay in milliseconds to apply. This is guaranteed
 * to be greater than or equal to 0 and less than the value returned by 
 * #ICE_SPDIFGetMaximumAudioDelay.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SPDIFSetAudioDelay(ICE_AudioHandle* handle, frost_int32 delay);

/**
 * @brief Set the S/PDIF Serial Copying Management System (SCMS) output bits.
 *
 * C-bit indicates whether the output is copyright asserted or public domain
 * L-bit indicates whether the output is original media or first generation 
 * copy or higher. See IEC 958 Digital Audio Interface Specification
 *
 * If the L-bit is set, a catagory code must be chosen from the list defined 
 * in IEC 60985-3, section 5.3.2.2.4, table 7. It is upto the implementor to
 * choose an appropriate catagory code.
 *
 * @param handle The handle for the audio device to which scms will be applied.
 * @param c_bit #frost_true if the C-bit should be set, #frost_false otherwise.
 * @param l_bit #frost_true if the L-bit should be set, #frost_false otherwise.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SPDIFSetSCMS(ICE_AudioHandle* handle, 
                            frost_bool c_bit, 
                            frost_bool l_bit);

#ifdef __cplusplus
}
#endif

#endif /* defined CABOT_ICE_ICE_SPDIF_H_INCLUDED */
