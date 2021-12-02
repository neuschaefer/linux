/**
 * Copyright 2008 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief ICE interface for DRM support.
 */ 

#ifndef CABOT_ICE_ICE_DRM_H_INCLUDED
#define CABOT_ICE_ICE_DRM_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_drm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the device specific DRM identifier.
 *
 * For DivX, this identifier is the registration code used to purchase content.
 *
 * @param type Type of device identifier to get.
 * @param buf Pointer to buffer that will hold the DRM identifier.
 * @param buf_len Size of the buffer. At most this many bytes of the identifier
 *                must be copied into supplied buffer.
 *
 * @return frost_true if the identifier was obtained successfully, otherwise frost_false.
 */
frost_bool ICE_DrmGetRegistrationCode(ICE_DrmDeviceIdType type, frost_uint8 * buf, frost_uint buf_len);

/**
 * @brief Get the device specific DRM deactivation identifier.
 *
 * For DivX, this identifier is the deactivation code used to deactivate device.
 *
 * @param type Type of device identifier to get.
 * @param buf Pointer to buffer that will hold the DRM identifier.
 * @param buf_len Size of the buffer. At most this many bytes of the identifier
 *                must be copied into supplied buffer.
 *
 * @return frost_true if the identifier was obtained successfully, otherwise frost_false.
 */
frost_bool ICE_DrmGetDeactivationCode(ICE_DrmDeviceIdType type, frost_uint8 * buf, frost_uint buf_len);

frost_bool ICE_DrmGetDeviceStatus(ICE_DrmDeviceStatus * drm_device_status);

/**
 * @brief Get the DRM status of the specified file.
 *
 * @param filespec  An absolute drive, partition, pathname and filename 
 *                  specifier for a local file, or a URL for a remote file.
 * @param drm_status DRM status of the file.
 *
 * @return frost_true if the status was set successfully, otherwise frost_false.
 */
frost_bool ICE_DrmGetFileStatus(const char * filespec, ICE_DrmStatus * drm_status);

/**
 * @brief Get DRM information for the specified file.
 *
 * This includes the current view count and the view count limit.
 * 
 * @param filespec  An absolute drive, partition, pathname and filename 
 *                  specifier for a local file, or a URL for a remote file.
 * @param file_info DRM information for the specified file.
 *
 * @return frost_true if the status was set successfully, otherwise frost_false.
 *
 * @note This function is expected to be called only for files which have 
 *       ice_drm_limited_views as their DRM status. For other DRM states,
 *       ICE_DrmFileInfo might not be fully set.
 * @note If a view count limit is not applicable for a file, it is advisable
 *       to set the field view_limit to 0.
 */
frost_bool ICE_DrmGetFileInfo(const char * filespec, ICE_DrmFileInfo * file_info);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_DRM_H_INCLUDED */
 
