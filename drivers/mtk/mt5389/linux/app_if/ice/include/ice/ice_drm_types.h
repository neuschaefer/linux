/**
 * Copyright 2008 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief ICE interface for DRM support types.
 */ 

#ifndef CABOT_ICE_ICE_DRM_TYPES_H_INCLUDED
#define CABOT_ICE_ICE_DRM_TYPES_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "frost/frost_basictypes.h"

/**
 * Byte length of DivX DRM registration code.
 */
#define ICE_DRM_DIVX_REGISTRATION_CODE_LENGTH 10
#define ICE_DRM_DIVX_DEACTIVATION_CODE_LENGTH 8


/**
 * Enumeration for the different types of DRM device IDs
 * which can be queried by ICE_DrmGetDeviceId.
 */
typedef enum ICE_DrmDeviceIdType
{
    /**
     * A DivX registration code uniquely identifies a device. It is used when
     * purchasing DRM-protected DivX content.
     */
    ice_drm_divx_invalid_code,
    ice_drm_divx_registration_code,
    ice_drm_divx_deactivation_code

} ICE_DrmDeviceIdType;

/**
 * Enumeration for DRM device states.
 */
typedef enum ICE_DrmDeviceStatus
{
    ice_drm_device_invalid,
    ice_drm_device_activated,
    ice_drm_device_deactivated
} ICE_DrmDeviceStatus;

/**
 * Enumeration for DRM states.
 */
typedef enum ICE_DrmStatus
{
    ice_drm_unlimited_views,    /**< The file is not restricted. */
    ice_drm_limited_views,      /**< The file can be viewed a limited number of times only. */
    ice_drm_rental_expired,     /**< The file cannot be viewed because rental expired.*/
    ice_drm_unauthorised_user,   /**< The file has been authorised for playback on another device. */
    ice_drm_not_supported_screen_size,    /**< The file has not supported screen size. */
    ice_drm_not_supported_frame_rate,    /**< The file has not supported frame rate. */
    ice_drm_not_supported_file_format,    /**< The file format has not supported by platform. */
    ice_drm_invalid_file_index    /**< The file has invalid index. */
} ICE_DrmStatus;

/**
 * @brief File specific DRM information.
 */
typedef struct ICE_DrmFileInfo
{
    frost_uint view_count; /**< The number of times the file has been played. */
    frost_uint view_limit; /**< The maximum number of times the file is allowed to be played. */
} ICE_DrmFileInfo;

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_DRM_TYPES_H_INCLUDED */
 
