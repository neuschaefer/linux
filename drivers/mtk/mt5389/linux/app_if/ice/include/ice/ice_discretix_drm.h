/**
 * @file ice_discretix_drm.h
 *
 * @brief This file holds the general high level ICE declarations, definitions
 *        and function prototypes neccessary for Discretix DRM (Digital Rights
 *        Management) decryption implementation.
 *
 * @note Copyright (c) 2011 Vestel
 *
 * All rights reserved
 */

#ifndef ICE_ICE_DISCRETIX_DRM_H_INCLUDED
#define ICE_ICE_DISCRETIX_DRM_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************/
/**
 * @name Type Definitions
 * @{
 */
/***************************************************************************/

/**
 * @brief Opaque type used to identify an open connection.
 */
typedef struct ICE_DiscretixDrmHandle ICE_DiscretixDrmHandle;

/**
 * @brief Type definition for DRM contents.
 */
typedef enum ICE_DiscretixDrmContentType
{
    ice_discretix_drm_unknown_content_type,
    ice_discretix_drm_stream
} ICE_DiscretixDrmContentType;

/**
 * @brief Type definition for possible DRM content locations.
 */
typedef enum ICE_DiscretixDrmContentLocationType
{
    ice_discretix_drm_unknown_content_position,
    ice_discretix_drm_internet,
    ice_discretix_drm_usb_storage
} ICE_DiscretixDrmContentLocationType;

/**
 * @brief Possible actions that can be taken on DRM contents.
 */
typedef enum ICE_DiscretixDrmActionType
{
    ice_discretix_drm_start,
    ice_discretix_drm_stop,
    ice_discretix_drm_pause,
    ice_discretix_drm_resume
} ICE_DiscretixDrmActionType;

/**
 * @brief Type definition for DRM rights.
 */
typedef enum ICE_DiscretixDrmRightType
{
    ice_discretix_drm_can_play       = 0x00000001,     /* If the file has the rights to play */
} ICE_DiscretixDrmRightType;

/**
 * @brief Type definition for possible analog output protections.
 */
typedef enum ICE_DiscretixDrmAnalogOutputProtectionType
{
    ice_discretix_drm_analog_output_free,
    ice_discretix_drm_set_cgms_a_to_11b,
    ice_discretix_drm_close_analog_output
} ICE_DiscretixDrmAnalogOutputProtectionType;

/**
 * @brief Type definition for possible SPDIF output protections.
 */
typedef enum ICE_DiscretixDrmSPDIFOutputProtectionType
{
    ice_discretix_drm_spdif_output_free,
    ice_discretix_drm_spdif_output_sgms_engaged,
    ice_discretix_drm_close_spdif_output
} ICE_DiscretixDrmSPDIFOutputProtectionType;


/***************************************************************************/
/**
 * @}
 * @name Discretix DRM Functions
 * @{
 */
/***************************************************************************/

/**
 * @brief Initialise the Discretix DRM library.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_DiscretixDrmInit();

/**
 * @brief Terminate the Discretix DRM library.
 */
void ICE_DiscretixDrmTerminate();

/**
 * @brief Open a connection to the Discretix DRM library by given URL.
 *        Return a handle for subsequent communications about the content.
 *
 * @param content_type The type of the content at given URL. This type will indicate
 *                     connection style applied to the content.
 *
 * @param url Address of the content.
 *
 * @param custom_data Custom data string. NULL pointer should be given for standard usage.
 *
 * @param service_url The URL which will be used for silent license acquisition.
 *                    NULL pointer should be used for standard usage.
 *
 * @return Returns a pointer to the handle of content at given URL on success,
 *         and a NULL pointer on failure.
 */
ICE_DiscretixDrmHandle * ICE_DiscretixDrmOpen(ICE_DiscretixDrmContentType content_type,
                                                const char * url,
                                                const char * custom_data,
                                                const char * service_url);

/**
 * @brief Close the connection of given handle, release related resources.
 *
 * @param handle The handle of content to be closed.
 */
void ICE_DiscretixDrmClose(ICE_DiscretixDrmHandle * handle);

/**
 * @brief Complete necessary processses for actions start, stop, pause and resume.
 *
 * @param handle The handle of content to be processed.
 *
 * @param action Action type to be processed.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_DiscretixDrmSetAction(ICE_DiscretixDrmHandle * handle, ICE_DiscretixDrmActionType action);

/**
 * @brief Check if the content at given handle is protected.
 *
 * @param handle The handle of content to be checked for protection.
 *
 * @return #frost_true if the content is protected, #frost_false otherwise.
 */
frost_bool ICE_DiscretixDrmIsProtected(ICE_DiscretixDrmHandle * handle);

/**
 * @brief Check content in terms of permissions.
 *
 * @param handle The handle of content to be checked.
 *
 * @param right Right type to be checked.
 *
 * @return #frost_true if there is a valid right, #frost_false otherwise.
 */
frost_bool ICE_DiscretixDrmCheckRight(ICE_DiscretixDrmHandle * handle, ICE_DiscretixDrmRightType right);

/**
 * @brief Install license for content at given handle.
 *
 * @param handle The handle of content to be licenced.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_DiscretixDrmInstallLicense(ICE_DiscretixDrmHandle * handle);

/**
 * @brief Get the URL of content at given handle.
 *
 * @param handle The handle of content to be looked for URL.
 *
 * @return Returns a character pointer to URL of content at given handle,
 *         and a NULL pointer on failure.
 */
const char * ICE_DiscretixDrmGetUrl(ICE_DiscretixDrmHandle * handle);

/**
 * @brief Get the unique ID of Television for Discfretix DRM Library usage.
 *
 * @return Returns a character pointer to unique ID of Television for Discfretix DRM Library usage.
 */
char * ICE_DiscretixDrmGetUniqueId();

/**
 * @brief Get what should be done for Anaolog output protection at given content handle.
 *
 * @param handle The handle of content to be queried.
 *
 * @return Returns analog output protection type of content.
 */
ICE_DiscretixDrmAnalogOutputProtectionType ICE_DiscretixDrmGetAnalogOutputProtectionType(ICE_DiscretixDrmHandle * handle);

/**
 * @brief Get what should be done for SPDIF output protection at given content handle.
 *
 * @param handle The handle of content to be queried.
 *
 * @return Returns SPDIF output protection type of content.
 */
ICE_DiscretixDrmSPDIFOutputProtectionType ICE_DiscretixDrmGetSPDIFOutputProtectionType(ICE_DiscretixDrmHandle * handle);

/**
 * @brief Gets the discretix drm enabled or not
 *
 * @return Returns the current status
 */
frost_bool ICE_DiscretixDrmEnabled(void);

/**
 * @brief read decrypted data from file
 *
 * @return Returns the current status
 */
frost_bool ICE_DiscretixDrmFileRead(ICE_DiscretixDrmHandle * handle, void* data, frost_uint32 dataSize, frost_uint32* dataActuallyRead);


frost_bool ICE_DiscretixDrmSetLicenseUrl(const char * LAUrl, unsigned long size);

const char * ICE_DiscretixDrmGetLicenseUrl();

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* ICE_ICE_DISCRETIX_DRM_H_INCLUDED */
