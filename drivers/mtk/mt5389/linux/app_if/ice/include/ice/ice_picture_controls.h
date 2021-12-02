/**
 * Copyright 2007 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for analogue TV picture control data types and functions.
 *
 * This file defines the common ICE data structures to allow manipulation
 * of analogue TV picture settings.
 */

#ifndef CABOT_ICE_ICE_PICTURE_CONTROLS_H_INCLUDED
#define CABOT_ICE_ICE_PICTURE_CONTROLS_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_video_types.h"
#include "config/fast_turn_on.h"
#include "config/include_project_median.h"
#include "config/include_project_phoenix.h"


#ifdef WIN32
#include "config/include_3d_support.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Possible video decoders to which picture settings can be applied.
 */
typedef enum ICE_VideoDecoderSet
{
    ice_primary_video_decoder,
    ice_secondary_video_decoder,
    ice_video_decoder_none
}ICE_VideoDecoderSet;

/**
 * @brief Type definition for colour temperature values.
 *
 */
typedef enum ICE_ColourTemperature
{
    ice_colour_temp_cool = 0,
    ice_colour_temp_normal = 1,
    ice_colour_temp_warm = 2,
    ice_colour_temp_pixelence = 3,
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
    ice_colour_temp_cool_3d = 4,
    ice_colour_temp_normal_3d = 5,
    ice_colour_temp_warm_3d = 6,
    ice_colour_temp_pixelence_3d = 7,
#endif
    
    ice_colour_temp_invalid = -1
}ICE_ColourTemperature;

/**
 * @brief Type definition for color enhancement modes.
 *
 */
typedef enum ICE_ColorEnhancement
{
    ice_color_enhancement_low = 0,
    ice_color_enhancement_medium = 1,
    ice_color_enhancement_high = 2,
    ice_color_enhancement_custom = 3,
    ice_color_enhancement_off = 4,
    ice_color_enhancement_invalid = -1
}ICE_ColorEnhancement;

/**
 * @brief Type definition for noise reduction modes.
 *
 */
typedef enum ICE_NoiseReduction
{
    ice_noise_reduction_off = 0,
    ice_noise_reduction_low = 1,
    ice_noise_reduction_medium = 2,
    ice_noise_reduction_high = 3,
    ice_noise_reduction_invalid = -1
}ICE_NoiseReduction;


typedef enum ICE_PowerSaveMode
{
    ice_power_save_mode_disabled=0,
    ice_power_save_mode_eco=1,
    ice_power_save_mode_screen_off=2,
    ice_power_save_mode_invalid=-1

}ICE_PowerSaveMode;






/**
 * @brief Common type definition for modes of picture enhancements.
 *
 */
typedef enum ICE_PictureEnhancementModes
{
    ice_picture_off = 0,
    ice_picture_low = 1,
    ice_picture_medium = 2,
    ice_picture_high = 3,
    ice_picture_demo = 4,
    ice_picture_invalid = -1
}ICE_PictureEnhancementModes;


/**
 * @brief Type definition for picture modes
 *
 */
typedef enum ICE_PictureMode
{
    ice_picture_mode_dynamic = 0, /*dynamic should be zero otherwise pqu mapings should be changed*/
    ice_picture_mode_natural,
    ice_picture_mode_cinema,
    ice_picture_mode_game,
    ice_picture_mode_pixellence,
    ice_picture_mode_sport,
    ice_picture_mode_user,
    /* Added new modes before this line. */
    ice_picture_mode_count   /** Number of different picture modes. */
} ICE_PictureMode;

/**
 * @brief Type definition for group of video sources where video settings applied
 * a mapping to this enum might be necessary based on actual eclipse source id
 */
typedef enum ICE_VideoSources
{
    ice_analog_rf_source,
    ice_digital_rf_source,
    ice_fav_source,
    ice_hdmi_source,
    ice_pc_source,
    ice_scart_cvbs_source,
    ice_scart_rgb_source,
    ice_svideo_source,
    ice_ypbpr_source,
    ice_media_browser_source,
    /* Added new sources before this line. */
    ice_video_source_count   /** Number of different picture modes. */
} ICE_VideoSources;

/**
 * @brief Type definition for group of video sources where video settings applied
 */
typedef enum ICE_VideoGroups
{
    /**
     * YC, RGB-SD (480i/576i), Component-SD (480i/576i) etc.
     */
    ice_sd_analogue,

    /**
     * DVB-SD, HDMI-SD (480i/576i) etc.
     */
    ice_sd_digital,

    /**
     * RGB-HD (video mode: e.g.1920x1080, 1280x720), Component-HD (video modes: e.g. 1920x1080, 1280x720)
     */
    ice_hd_analogue,

    /**
     * DVB-HD, HDMI-HD, Media (usb) video etc.
     */
    ice_hd_digital,

    /**
     * PC modes e.g. 800x600, 1024x768, 1280x1024
     */
    ice_pc,

    /**
     * USB pictures
     */
    ice_photo,

    /* Added new groups before this line. */
    ice_group_max   /** Number of different picture modes. */
} ICE_VideoGroups;

/**
 * @brief Type definition for back light mode.
 *
 *
 */
typedef enum ICE_BacklightDimmingMode
{
    ice_backlight_dimming_low = 0, /* fixed low backlight value */
    ice_backlight_dimming_medium = 1,  /* fixed medium backlight value */
    ice_backlight_dimming_high = 2,  /* fixed high backlight value */
    ice_backlight_dimming_auto = 3, /* auto mode implementation, based on vendor capabilities */
    ice_backlight_dimming_off = 4, /* screen off */
    ice_backlight_dimming_power_save = 5, /* ice implementation is a fixed backlight value , like in low, mid, high cases*/
    ice_backlight_dimming_ambilight = 6, /*Ambilight Sensor active*/    
    ice_backlight_dimming_invalid = -1
}ICE_BacklightDimmingMode;

/**
 * @brief Type definition for film mode.
 *
 *
 */
typedef enum ICE_FilmMode
{
    ice_film_mode_auto = 0, 
    ice_film_mode_off = 1,
    ice_film_mode_invalid = -1
}ICE_FilmMode;

/**
 * @brief Type definition for film mode.
 *
 *
 */
typedef enum ICE_ColorGainType
{
    ice_color_gain_red = 0, 
    ice_color_gain_green = 1,
    ice_color_gain_blue = 2,
    ice_color_gain_invalid = -1
}ICE_ColorGainType;

/**
 * @brief RGB gain parameters
 *
 *
 */
typedef struct ICE_ColorGainParams
{
    frost_uint16 Red_Gain;
    frost_uint16 Green_Gain;
    frost_uint16 Blue_Gain;
}ICE_ColorGainParams;

/**
 * @brief Type definition dynamic contrast
 *
 *
 */
typedef enum ICE_DynamicContrast
{
    ice_dynamic_contrast_low = 0,
    ice_dynamic_contrast_medium = 1,
    ice_dynamic_contrast_high = 2,
    ice_dynamic_contrast_off = 3,

    ice_dynamic_contrast_invalid = -1
}ICE_DynamicContrast;


/*
** source_index used by the ICE_PictureControlGet functions.
** It is 0 for the TV tuner and 1 for the first external source,
** up to the value returned by ICE_ExternalNumberOfInterfaces()
** which means the last external source.
*/

/**
 * @brief Get the minimum and maximum values for contrast.
 *
 * @param decoder Video device to which settings will be applied.
 * @param min_level Minimum contrast value that will be filled in by this function.
 * @param max_level Maximum contrast value that will be filled in by this function.
 *
 * @note  the min and max value are the linear scale used by the OSD and the user
 *        has the ability to increment or decrement the current setting between and
 *        incluiding these limits.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
 frost_bool ICE_PictureControlGetContrastRange(ICE_VideoDecoderSet decoder,
                                               frost_int* min_level, frost_int* max_level);


/**
 * @brief Set the contrast to the specified level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param contrast_level The contrast level that will be applied.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_bool ICE_PictureControlSetContrast(ICE_VideoDecoderSet decoder, frost_int contrast_level, frost_bool is_3d_mode);
#else
frost_bool ICE_PictureControlSetContrast(ICE_VideoDecoderSet decoder, frost_int contrast_level);
#endif


/**
 * @brief Get the default contrast level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param source_index Source to which settings will be applied.
 * @param picture_mode Which picture mode's default value will be get.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return Current contrast level.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
float ICE_PictureControlGetDefaultContrast(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode, frost_bool is_3d_mode);
#else 
float ICE_PictureControlGetDefaultContrast(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode);
#endif


/**
 * @brief Get the minimum and maximum values for brightness.
 *
 * @param decoder Video device to which settings will be applied.
 * @param min_level Minimum brightness value that will be filled in by this function.
 * @param max_level Maximum brightness value that will be filled in by this function.
 *
 * @note  the min and max value are the linear scale used by the OSD and the user
 *        has the ability to increment or decrement the current setting between and
 *        incluiding these limits.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_PictureControlGetBrightnessRange(ICE_VideoDecoderSet decoder,
                                                           frost_int* min_level, frost_int* max_level);


/**
 * @brief Set the brightness to the specified level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param brightness_level The brightness level that will be applied.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_bool ICE_PictureControlSetBrightness(ICE_VideoDecoderSet decoder, frost_int brightness_level, frost_bool is_3d_mode);
#else 
frost_bool ICE_PictureControlSetBrightness(ICE_VideoDecoderSet decoder, frost_int brightness_level);
#endif


/**
 * @brief Get the default brightness level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param source_index Source to which settings will be applied.
 * @param picture_mode Which picture mode's default value will be get.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return Current brightness level.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
float ICE_PictureControlGetDefaultBrightness(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode, frost_bool is_3d_mode);
#else 
float ICE_PictureControlGetDefaultBrightness(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode);
#endif


/**
 * @brief Get the minimum and maximum values for sharpness.
 *
 * @param decoder Video device to which settings will be applied.
 * @param min_level Minimum sharpness value that will be filled in by this function.
 * @param max_level Maximum sharpness value that will be filled in by this function.
 *
 * @note  the min and max value are the linear scale used by the OSD and the user
 *        has the ability to increment or decrement the current setting between and
 *        incluiding these limits.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_PictureControlGetSharpnessRange(ICE_VideoDecoderSet decoder,
                                                           frost_int* min_level, frost_int* max_level);


/**
 * @brief Set the sharpness to the specified level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param sharpness_level The sharpness level that will be applied.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_bool ICE_PictureControlSetSharpness(ICE_VideoDecoderSet decoder, frost_int sharpness_level, frost_bool is_3d_mode);
#else 
frost_bool ICE_PictureControlSetSharpness(ICE_VideoDecoderSet decoder, frost_int sharpness_level);
#endif


/**
 * @brief Get the default sharpness level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param source_index Source to which settings will be applied.
 * @param picture_mode Which picture mode's default value will be get.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return Current sharpness level.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
float ICE_PictureControlGetDefaultSharpness(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode, frost_bool is_3d_mode);
#else 
float ICE_PictureControlGetDefaultSharpness(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode);
#endif


/**
 * @brief Get the minimum and maximum values for colour.
 *
 * @param decoder Video device to which settings will be applied.
 * @param min_level Minimum colour value that will be filled in by this function.
 * @param max_level Maximum colour value that will be filled in by this function.
 *
 * @note  the min and max value are the linear scale used by the OSD and the user
 *        has the ability to increment or decrement the current setting between and
 *        incluiding these limits.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_PictureControlGetColourRange(ICE_VideoDecoderSet decoder,
                                                     frost_int* min_level, frost_int* max_level);


/**
 * @brief Set the colour to the specified level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param color_level The colour level that will be applied.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_bool ICE_PictureControlSetColour(ICE_VideoDecoderSet decoder, frost_int color_level, frost_bool is_3d_mode);
#else 
frost_bool ICE_PictureControlSetColour(ICE_VideoDecoderSet decoder, frost_int colour_level);
#endif


/**
 * @brief Get the default colour level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param source_index Source to which settings will be applied.
 * @param picture_mode Which picture mode's default value will be get.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return Current colour level.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
float ICE_PictureControlGetDefaultColour(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode, frost_bool is_3d_mode);
#else 
float ICE_PictureControlGetDefaultColour(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode);
#endif


/**
 * @brief Get the minimum and maximum values for hue.
 *
 * @param decoder Video device to which settings will be applied.
 * @param min_level Minimum hue value that will be filled in by this function.
 * @param max_level Maximum hue value that will be filled in by this function.
 *
 * @note  the min and max value are the linear scale used by the OSD and the user
 *        has the ability to increment or decrement the current setting between and
 *        incluiding these limits.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_PictureControlGetHueRange(ICE_VideoDecoderSet decoder,
                                                   frost_int* min_level, frost_int* max_level);


/**
 * @brief Set the hue to the specified level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param hue_level The hue level that will be applied.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_bool ICE_PictureControlSetHue(ICE_VideoDecoderSet decoder, frost_int hue_level, frost_bool is_3d_mode);
#else 
frost_bool ICE_PictureControlSetHue(ICE_VideoDecoderSet decoder, frost_int hue_level);
#endif


/**
 * @brief Get the default hue level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param source_index Source to which settings will be applied.
 * @param picture_mode Which picture mode's default value will be get.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return Current hue level.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
float ICE_PictureControlGetDefaultHue(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode, frost_bool is_3d_mode);
#else 
float ICE_PictureControlGetDefaultHue(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode);
#endif


/**
 * @brief Get the minimum and maximum values for skin tone.
 *
 * @param decoder Video device to which settings will be applied.
 * @param min_level Minimum skin tone value that will be filled in by this function.
 * @param max_level Maximum skin tone value that will be filled in by this function.
 *
 * @note  the min and max value are the linear scale used by the OSD and the user
 *        has the ability to increment or decrement the current setting between and
 *        incluiding these limits.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_PictureControlGetSkinToneRange(ICE_VideoDecoderSet decoder,
                                                   frost_int* min_level, frost_int* max_level);


/**
 * @brief Set the skin tone to the specified level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param skin_tone_level The skin tone level that will be applied.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_bool ICE_PictureControlSetSkinTone(ICE_VideoDecoderSet decoder, frost_int skin_tone_level, frost_bool is_3d_mode);
#else
frost_bool ICE_PictureControlSetSkinTone(ICE_VideoDecoderSet decoder, frost_int skin_tone_level);
#endif


/**
 * @brief Get the default skin tone level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param source_index Source to which settings will be applied.
 * @param picture_mode Which picture mode's default value will be get.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return Current skin tone level.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_int ICE_PictureControlGetDefaultSkinTone(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode, frost_bool is_3d_mode);
#else 
frost_int ICE_PictureControlGetDefaultSkinTone(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode);
#endif
/**
 * @brief Get the default red rgb gain level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param source_index Source to which settings will be applied.
 * @param picture_mode Which picture mode's default value will be get.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return Current red rgb gain level.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_int ICE_PictureControlGetDefaultRGBRedGain(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode, frost_bool is_3d_mode);
#else
frost_int ICE_PictureControlGetDefaultRGBRedGain(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode);
#endif

/**
 * @brief Get the default green rgb gian level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param source_index Source to which settings will be applied.
 * @param picture_mode Which picture mode's default value will be get.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return Current green rgb gain level.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_int ICE_PictureControlGetDefaultRGBGreenGain(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode, frost_bool is_3d_mode);
#else
frost_int ICE_PictureControlGetDefaultRGBGreenGain(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode);
#endif


/**
 * @brief Get the default blue rgb gian level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param source_index Source to which settings will be applied.
 * @param picture_mode Which picture mode's default value will be get.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return Current blue rgb gain level.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_int ICE_PictureControlGetDefaultRGBBlueGain(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode, frost_bool is_3d_mode);
#else
frost_int ICE_PictureControlGetDefaultRGBBlueGain(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode);
#endif


/**
 * @brief Get the minimum and maximum values for rgb gain.
 *
 * @param decoder Video device to which settings will be applied.
 * @param min_level Minimum rgb gain value that will be filled in by this function.
 * @param max_level Maximum rgb gain value that will be filled in by this function.
 *
 * @note  the min and max value are the linear scale used by the OSD and the user
 *        has the ability to increment or decrement the current setting between and
 *        incluiding these limits.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_PictureControlGetRGBGainRange(ICE_VideoDecoderSet decoder,
                                                   frost_int* min_level, frost_int* max_level);

/**
 * @brief Set the rgb gain to the specified level.
 *
 * @param decoder Video device to which settings will be applied.
 * @param rgb_gain_level The rgb gain level that will be applied.
 * @param color_gain The color gain  that will be applied.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_bool ICE_PictureControlSetRGBGain(ICE_VideoDecoderSet decoder, frost_int rgb_gain_level, ICE_ColorGainType color_gain, frost_bool is_3d_mode);
#else
frost_bool ICE_PictureControlSetRGBGain(ICE_VideoDecoderSet decoder, frost_int rgb_gain_level, ICE_ColorGainType color_gain);
#endif


/**
 * @brief Set the colour temperature to the specified value.
 *
 * @param decoder Video device to which settings will be applied.
 * @param colour_temperature The colour temperature that will be applied.
 * Possible colour temperatures are ice_colour_temp_cool, ice_colour_temp_normal,
 * ice_colour_temp_warm.
 * @param color_gain_params The color gain parameters that will be applied.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_bool ICE_PictureControlSetColourTemperature(ICE_VideoDecoderSet decoder, ICE_ColourTemperature colour_temperature, ICE_ColorGainParams *color_gain_params, frost_bool is_3d_mode);
#else
frost_bool ICE_PictureControlSetColourTemperature(ICE_VideoDecoderSet decoder, ICE_ColourTemperature colour_temperature, ICE_ColorGainParams *color_gain_params);
#endif


/**
 * @brief Get default current colour temperature of the given picture mode.
 *
 * @param decoder Video device to which settings will be applied.
 * @param source_index Source to which settings will be applied.
 * @param picture_mode which picture mode's default value will be get.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return Current colour temperature. Possible values are ice_colour_temp_cool,
 * ice_colour_temp_normal, ice_colour_temp_warm.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
ICE_ColourTemperature ICE_PictureControlGetDefaultColourTemperature(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode, frost_bool is_3d_mode);
#else 
ICE_ColourTemperature ICE_PictureControlGetDefaultColourTemperature(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode);
#endif

/**
 * @brief Set the color level from picture wizard to the specified value.
 *
 * @param decoder Video device to which settings will be applied.
 * @param color_level The color level that will be applied.
 * Possible types are soft, original, strong
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_PictureControlSetWizardColor(ICE_VideoDecoderSet decoder, frost_int color_level);

/**
 * @brief Set the contrast level from picture wizard to the specified value.
 *
 * @param decoder Video device to which settings will be applied.
 * @param contrast_level The contrast level that will be applied.
 * Possible types are soft, original, strong
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_PictureControlSetWizardContrast(ICE_VideoDecoderSet decoder, frost_int contrast_level);

/**
 * @brief Set the sharpness level from picture wizard to the specified value.
 *
 * @param decoder Video device to which settings will be applied.
 * @param sharpness_level The sharpness level/mode that will be applied.
 * Possible types are soft, original, strong
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_PictureControlSetWizardSharpness(ICE_VideoDecoderSet decoder, frost_int sharpness_level);

/**
 * @brief Set the movie sense to the specified value.
 *
 * @param movie_sense The movie sense level/mode that will be applied.
 * Possible types are off, low, mid, high, demo
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_PictureControlSetMovieSense(ICE_PictureEnhancementModes movie_sense);

/**
 * @brief Set the pixellence to the specified value. Depending on the "Video Engineer"'s requirements or application there may 
 * be only on/off/demo modes. In that case, any of "low/mid/high" modes can be used just to set Pixellence "on".
 *
 * @param pixellence_mode The pixellence level/mode that will be applied.
 * Possible types are off, low, mid, high, demo
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_bool ICE_PictureControlSetPixellence(ICE_PictureEnhancementModes pixellence_mode, frost_bool is_3d_mode);
#else 
frost_bool ICE_PictureControlSetPixellence(ICE_PictureEnhancementModes pixellence_mode);
#endif

#if defined(FAST_TURN_ON) && (defined(INCLUDE_PROJECT_MEDIAN) || defined(INCLUDE_PROJECT_PHOENIX))
frost_bool ICE_PictureControlSetPictureSettingsApplied(frost_bool applied);
#endif
/**
 * @brief Set the noise reduction to the specified value.
 *
 * @param decoder Video device to which settings will be applied.
 * @param noise_reduction The noise reduction value that will be applied.
 * Possible noise reduction types are ice_noise_reduction_off, ice_noise_reduction_low,
 * ice_noise_reduction_medium, ice_noise_reduction_high.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_PictureControlSetNoiseReduction(ICE_VideoDecoderSet decoder, ICE_NoiseReduction noise_reduction);


/**
 * @brief Get the current noise reduction level.
 *
 * @param decoder Video device to which settings will be applied.
 *
 * @return Current noise reduction level. Possible values are ice_noise_reduction_off,
 * ice_noise_reduction_low, ice_noise_reduction_medium, ice_noise_reduction_high
 */
ICE_NoiseReduction ICE_PictureControlGetNoiseReduction(ICE_VideoDecoderSet decoder);

/**
 * @brief Enable / Disable blue background.
 *
 * @param decoder Video device to which settings will be applied.
 * @param enable If #frost_true, enable blue background; if #frost_false, disable blue background.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_PictureControlEnableBlueBackground(ICE_VideoDecoderSet decoder, frost_bool enable);


/**
 * @brief Get the current blue background state.
 *
 * @param decoder Video device to which settings will be applied.
 *
 * @return #frost_true if enabled, #frost_false otherwise.
 */
frost_bool ICE_PictureControlIsBlueBackgroundEnabled(ICE_VideoDecoderSet decoder);


/**
 * @brief Set OSD brightness to the specifed level.
 *
 * @param brightness_level Brightness level that will be applied.
 *
 * @return #frost_true on success, #frost_false otherwise.
 */
frost_bool ICE_PictureControlSetOSDBrightness(frost_int brightness_level);


/**
 * @brief Get current OSD brightness level.
 *
 * @return Current OSD brightness level.
 */
frost_int ICE_PictureControlGetOSDBrightness(void);


/**
 * @brief Enables or disables film mode.
 *
 * @param decoder Video device to which settings will be applied.
 * @param film_mode If #frost_true, enable film mode,  if #frost_false, disable film mode.
 *
 * @return #frost_true on success, #frost_false otherwise.
 */
frost_bool ICE_PictureControlSetFilmMode(ICE_VideoDecoderSet decoder, ICE_FilmMode film_mode);

/**
 * @brief Get the current film mode  status.
 *
 * @param decoder Video device to which settings will be applied
 *
 * @return #frost_true on success, #frost_false otherwise.
 */
frost_bool ICE_PictureControlIsFilmModeEnabled(ICE_VideoDecoderSet decoder);

/**
 * @brief Set necessary user style specific changes which are not set by the application.
 * Application should call this function before any other ice picture interface call.
 * Depending on concept, platform may perform different operations with respect to selected picture mode.
 * ie: on tv543, although the noise reduction level is the same such as medium, platform applies different noise reduction levels for different picture  modes.
 *
 * @note  this function is called after the AV source has been selected and before any
 *        other ICE_PictureControl functions are called that effect the picture quality.
 *
 * @note  If the decoder is the secondary decoder then either "picture in picture" or
 *        "picture and picture" is in the process of being presented to the user.
 *
 * @param decoder Video device to which settings will be applied.
 * @param picture_mode the selected user style
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return #frost_true on success, #frost_false otherwise.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_bool ICE_PictureControlSetPictureMode(ICE_VideoDecoderSet decoder, ICE_PictureMode picture_mode, frost_bool is_3d_mode);
#else 
frost_bool ICE_PictureControlSetPictureMode(ICE_VideoDecoderSet decoder, ICE_PictureMode picture_mode);
#endif

/**
 * @brief Get the current 3D Comb Filter state.
 *
 * @param decoder Video device to which settings will be applied.
 *
 * @return #frost_true if enabled, #frost_false otherwise.
 */
frost_bool ICE_PictureControlIs3DCombFilterEnabled(ICE_VideoDecoderSet decoder);

/**
 * @brief Enable / Disable 3D Comb Filter.
 *
 * @param decoder Video device to which settings will be applied.
 * @param enable If #frost_true, enable 3D Comb Filter; if #frost_false, disable 3D Comb Filter.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_PictureControlEnable3DCombFilter(ICE_VideoDecoderSet decoder, frost_bool enable);


/**
 * @brief Enable / Disable HDMI True Black.
 *
 * @note it will just work in hdmi mode.It is ineffective within other sources.
 *
 * @param enable If #frost_true, enable HDMI True Black; if #frost_false, disable HDMI True Black.
 * @param is3D indicates whether the device is in 3D mode or not.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_bool ICE_PictureControlEnableHDMITrueBlack(frost_bool enable, frost_bool is3D);
#else
frost_bool ICE_PictureControlEnableHDMITrueBlack(frost_bool enable);
#endif

/**
 * @brief Enable / disable backlight of a TV panel from HW, over GPIO.
 * 
 * @param enable Enable backlight pin if #frost_true, disable if #frost_false.
 * 
 * @return #frost_true on success, #frost_false otherwise. 
 *
 */
frost_bool ICE_BacklightPinEnable(frost_bool enable);

/**
 * @brief Set backlight mode state
 *
 * @param dimming_mode Back light dimming mode selection.
 *
 * @note power save mode is nothing different then low,medium, high cases. It is a fixed value setting to 
 * backlight, auto mode implementation might be different based on vendor capabilities.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_PictureControlSetBacklightDimmingMode(ICE_BacklightDimmingMode dimming_mode);

/**
 * @brief Set dynamic contrast mode state
 *
 * @param decoder Video device to which settings will be applied.
 * @param dynamic_contrast Dynamic contrast level selection
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
 #if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
frost_bool ICE_PictureControlSetDynamicContrast(ICE_VideoDecoderSet decoder, ICE_DynamicContrast dynamic_contrast, frost_bool is_3d_mode);
 #else
frost_bool ICE_PictureControlSetDynamicContrast(ICE_VideoDecoderSet decoder, ICE_DynamicContrast dynamic_contrast);
#endif

/**
 * @brief Get the default dynamic contrast
 *
 * @param decoder Video device to which settings will be applied.
 * @param source_index Source to which settings will be applied.
 * @param picture_mode Which picture mode's default value will be get.
 * @param is_3d_mode indicates whether the device is in 3D mode or not.
 *
 * @return Default dynamic contrast level for requested source & picture mode
 */
#if (defined INCLUDE_3D_SUPPORT) || (defined ENABLE_3D)
ICE_DynamicContrast ICE_PictureControlGetDefaultDynamicContrast(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode, frost_bool is_3d_mode);
#else 
ICE_DynamicContrast ICE_PictureControlGetDefaultDynamicContrast(ICE_VideoDecoderSet decoder, frost_int source_index, ICE_PictureMode picture_mode);
#endif
/**
 * @brief Get the default film mode
 *
 * @param decoder Video device to which settings will be applied.
 *
 * @return Default dynamic contrast level for requested source & picture mode
 */
ICE_FilmMode ICE_PictureControlGetDefaultFilmMode(ICE_VideoDecoderSet decoder);

/**
 * @brief 
 *
 * @param decoder Video device to which settings will be applied.
 * @param color to be applied
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_ApplyVideoPatterns(ICE_VideoDecoderSet decoder, frost_uint32 color);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_PICTURE_CONTROLS_H_INCLUDED*/

