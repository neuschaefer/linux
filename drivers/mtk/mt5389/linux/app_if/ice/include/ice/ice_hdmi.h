/**
 * Copyright 2003-8 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Types and functions to interface with the HDMI interface.
 */

#ifndef CABOT_ICE_ICE_HDMI_H_INCLUDED
#define CABOT_ICE_ICE_HDMI_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_audio_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief HDMI video output resolutions.
 */
typedef enum ICE_HDMIOutputResolution
{
    ICE_HDMI_Invalid_Resolution     = 0x00,
    ICE_HDMI_576i_Resolution        = 0x01,
    ICE_HDMI_576p_Resolution        = 0x02,
    ICE_HDMI_720p_Resolution        = 0x04,
    ICE_HDMI_1080i_Resolution       = 0x08,
    ICE_HDMI_1080p_Resolution       = 0x10

} ICE_HDMIOutputResolution;

/**
 * @brief HDMI video output colour spaces.
 */
typedef enum ICE_HDMIOutputColourSpace
{
    ICE_HDMI_RGB_Colour_Space,
    ICE_HDMI_YCbCr_Colour_Space

} ICE_HDMIOutputColourSpace;

/**
 * @brief Aspect ratio of television connected to HDMI.
 */
typedef enum ICE_HDMITVAspectRatio
{
    ICE_HDMI_TV_aspect_4_3,
    ICE_HDMI_TV_aspect_16_9,
    ICE_HDMI_TV_aspect_auto
    
} ICE_HDMITVAspectRatio;

/**
 * @brief HDMI cable states.
 */
typedef enum ICE_HDMICableState
{
    ICE_HDMI_Cable_Plugged_In_Check_Edid,
    ICE_HDMI_Cable_Plugged_In,
    ICE_HDMI_Cable_Just_Unplugged,
    ICE_HDMI_Cable_Unplugged

} ICE_HDMICableState;

/**
 * @brief HDMI operations can return these results.
 */
typedef enum ICE_HDMIOperationResult
{
    ICE_HDMIOperationSuccess,
    ICE_HDMIOperationFail,
    ICE_HDMIOperationNotSupported

} ICE_HDMIOperationResult;

/**
 * @brief Initialise the HDMI video output devices, scaler and transmitter.
 *
 * Default start mode of scaler is 576p 4-3.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_HDMIInit(void);

/**
 * @brief Sets the HDMI video output resolution (e.g. 576p, 720p ...).
 *
 * @param HDMI_video_output_resolution_mode The new HDMI video output
 *                                          resolution.
 * @param do_not_use_delay_and_mute         Idetifies the selection between
 *                                          speed and quality of mode switch.
 *                                          #frost_true disables smooth switches
 *                                          on HDMI output (audio/video
 *                                          anomalies may occur but switch is
 *                                          faster). This mode can be used when
 *                                          the audio and video are off and
 *                                          switching time is important, e.g.
 *                                          when initializing.
 *                                          #frost_false for regular mode switch
 *                                          procedure.
 * @param control_for_support               Specifies whether a control for mode
 *                                          support will be done or not.
 *
 * @return ICE_HDMIOperationSuccess on success, ICE_HDMIOperationFail on failure
 *         or ICE_HDMIOperationNotSupported if not supported.
 */
ICE_HDMIOperationResult ICE_HDMISetResolution(
    ICE_HDMIOutputResolution HDMI_video_output_resolution_mode,
    frost_bool do_not_use_delay_and_mute,
    frost_bool control_for_support);

/**
 * @brief Sets a HDMI video output colour space (RGB, YCbCr).
 *
 * @param HDMI_video_output_colour_space_mode   The new HDMI video output
 *                                              colour space.
 *
 * @return ICE_HDMIOperationSuccess on success, ICE_HDMIOperationFail on
 *         failure, or ICE_HDMIOperationNotSupported if not supported.
 */
ICE_HDMIOperationResult ICE_HDMISetOutputColourSpace(
    ICE_HDMIOutputColourSpace HDMI_video_output_colour_space_mode);

/**
 * @brief Returns current HDMI output colour space (RGB, YCbCr).
 *
 * @return The current HDMI output colour space.
 */
ICE_HDMIOutputColourSpace ICE_HDMIGetCurrentOutputColourSpace(void);

/**
 * @brief Sets HDMI video output aspect ratio (4-3 or 16-9).
 *
 * @param HDMI_video_output_aspect_ratio    The required aspect ratio of HDMI
 *                                          video output (4-3 or 16-9).
 * @param do_not_use_delay_and_mute         Idetifies the selection between
 *                                          speed and quality of mode switch.
 *                                          #frost_true disables smooth switches
 *                                          on HDMI output (audio/video
 *                                          anomalies may occur but switch is
 *                                          faster). This mode can be used when
 *                                          the audio and video are off and
 *                                          switching time is important, e.g.
 *                                          when initializing.
 *                                          #frost_false for regular mode switch
 *                                          procedure.
 *
 * @return ICE_HDMIOperationSuccess on success, ICE_HDMIOperationFail on
 *         failure, or ICE_HDMIOperationNotSupported if not supported.
 */
ICE_HDMIOperationResult ICE_HDMISetAspectRatio(
    ICE_HDMITVAspectRatio HDMI_video_output_aspect_ratio,
    frost_bool do_not_use_delay_and_mute);

/**
 * @brief Returns current HDMI video output resolution (e.g. 576p, 720p ...).
 *
 * @return The current HDMI video output resolution .
 */
ICE_HDMIOutputResolution ICE_HDMIGetCurrentHDMIVideoOutputResolution(void);

/**
 * @brief Returns supported HDMI video output resolutions (e.g. 576p, 720p ...).
 *
 * @return The supported HDMI video output resolutions.
 */
ICE_HDMIOutputResolution ICE_HDMIGetSupportedVideoOutputResolutions(void);

/**
 * @brief Returns the supported HDMI video input resolutions reported by the
 * attached sink (e.g. TV)
 *
 * @return The supported HDMI video input resolutions of the sink device.
 */
ICE_HDMIOutputResolution ICE_HDMIGetSinkSupportedVideoResolutions(void);

/**
 * @brief Returns the native HDMI video input resolutions reported by the
 * attached sink (e.g. TV)
 *
 * @return The native HDMI video input resolutions of the sink device.
 */
ICE_HDMIOutputResolution ICE_HDMIGetSinkNativeVideoResolution(void);

/**
 * @brief Sets current HDMI video output resolution (e.g. 576p, 720p ...).
 *
 * @param video_output_resolution The current HDMI video output resolution .
 */
void ICE_HDMISetCurrentHDMIVideoOutputResolution(
    ICE_HDMIOutputResolution video_output_resolution);

/**
 * @brief Returns TV aspect ratio of current HDMI video output resolution
 *        (e.g. ICE_HDMI_TV_aspect_4_3 ...).
 *
 * @return The current aspect ratio applied by HDMI video output resolution
 *         register set.
 */
ICE_HDMITVAspectRatio ICE_HDMIGetCurrentAspectRatio(void);

/**
 * @brief Sets aspect ratio of current HDMI video output resolution
 *        (e.g. ICE_HDMI_TV_aspect_4_3 ...).
 *
 * @param aspect_ratio      The current aspect ratio applied by HDMI video
 *                          output resolution register set.
 */
void ICE_HDMISetCurrentAspectRatio(ICE_HDMITVAspectRatio aspect_ratio);

/**
 * @brief Sets TV aspect ratio of current HDMI video output resolution
 *        (e.g. ICE_HDMI_TV_aspect_4_3 ...).
 *
 * @param tv_aspect_ratio The current tv aspect ratio chosen by user.
 */
void ICE_HDMISetTVAspectRatio(ICE_HDMITVAspectRatio tv_aspect_ratio);

/**
 * @brief Resets transmitter (Hardware Reset).
 */
void ICE_HDMIHardwareResetTransmitter(void);

/**
 * @brief Resets scaler (Hardware Reset).
 */
void ICE_HDMIHardwareResetScaler(void);

/**
 * @brief Resets both scaler and transmitter at the same time (Hardware Reset).
 */
void ICE_HDMIHardwareResetScalerAndTransmitter(void);

/**
 * @brief Mutes HDMI output, both audio and video.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_HDMIMute(void);

/**
 * @brief Unmutes HDMI output.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_HDMIUnmute(void);

/**
 * @brief Gets HDMI output availability.
 *
 * @return #frost_true if available, #frost_false if not available.
 */
frost_bool ICE_HDMIGetOutputAvailability(void);

/**
 * @brief Enable/Disable HDCP
 *
 * @param enable  Determines HDCP check,
 *                #frost_false disables HDCP check
 *                #frost_true  enables  HDCP check
 * 
 */
void ICE_HDMIHDCPEnableDisable(frost_bool enable);

/**
 * @brief Receive System Renewability Message (SRM) data for HDCP.
 *
 * This data may have been received from a CI+ Module or other means and should
 * be applied to the HDCP function. The SRM is used to perform the function
 * of a blacklist for HDCP. See HDCP specification v 1.3, 21 December 2006.
 *
 * @param data The SRM data as per HDCP specification v 1.3, 21 December 2006.
 * @param size The size of the SRM data in bytes.
 *
 * @return #frost_true if the SRM data was processed successfully.
 * 
 */
frost_bool ICE_HDMIHDCPReceiveSRM(const frost_uint8* data, frost_int size);

/**
 * @brief Set the HDMI audio output type.
 *
 * @param audio_digital_out_type The audio output type (for example, PCM).
 */
void ICE_HDMISetOutputType(ICE_AudioDigitalOut audio_digital_out_type);

#ifdef __cplusplus
}
#endif

#endif /* defined CABOT_ICE_ICE_HDMI_H_INCLUDED */
