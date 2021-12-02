/**
 * Copyright 2006 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for handling removable devices.
 *
 * This interface is used by Eclipse to be notified when the status of
 * a removable device is changed.
 */

#ifndef CABOT_ICE_ICE_REMOVABLE_DEVICE_H_INCLUDED
#define CABOT_ICE_ICE_REMOVABLE_DEVICE_H_INCLUDED

#include "config/include_follow_tv.h"

#include "frost/frost_basictypes.h"
#include "ice_filesystem_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Possible states for removable devices.
 *
 */
typedef enum ICE_RemovableDeviceStatus
{
    ICE_RemovableDeviceStatusConnectedReadOnly,
    ICE_RemovableDeviceStatusConnectedReadWrite,
    ICE_RemovableDeviceStatusDisconnected
} ICE_RemovableDeviceStatus;

/**
 * Possible types for removable devices.
 */
typedef enum ICE_RemovableDeviceType
{
    ICE_RemovableDeviceUSB,
    ICE_RemovableDeviceDLNA,
#ifdef INCLUDE_FOLLOW_TV
    ICE_RemovableDeviceDLNA_DMR,
#endif
    ICE_RemovableDeviceNotSpecified
} ICE_RemovableDeviceType;

/**
 * @brief Prototype for the callback function.
 *
 * The ICE implementation should call this function whenever the status of a
 * a removable device is changed.  It is also called at boot, for the
 * device(s) that are initially plugged in (if any).
 *
 * @param user_data Data to be passed back when the callback is raised.
 *        The user_data is given to #ICE_RemovableDeviceInit.
 * @param status New status of removable device.
 * @param name pointer to an identifier string for the device
 * @param drive_number drive number assigned to the device
 */
typedef void (*ICE_RemovableDeviceCallback)(void* user_data,
    ICE_RemovableDeviceStatus status,
    const char* name,
    ICE_RemovableDeviceType device_type,
    frost_uint drive_number,
    fs_type_t file_system_type);

/**
 * @brief Initialise removable device subsystem.
 *
 * Called before any other ICE removable device functions are called.
 *
 * This must immediately generate callbacks for any removable devices
 * that are already plugged in.  This may be done by calling the callback
 * directly from inside this function, or by arranging for it to happen
 * in a separate thread.
 *
 * @param user_data Data to be passed back when the callback is raised.
 * @param callback function to be called when removable device state changes.
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note If called when already initialised, return #frost_true.
 */
frost_bool ICE_RemovableDeviceInit(void* user_data,
                                   ICE_RemovableDeviceCallback callback);

/**
 * @brief Get the drive number assigned to the first removable drive
 *
 * @return First removable drive number or -1 in case of an error.
 */
frost_int ICE_GetFirstRemovableDriveNumber(void);

/**
 * @brief Copy files NVRAM files From the first removable drive
 *
 * @return true if operation is successfull, false otherwise.
 */
frost_bool ICE_CopyFilesFromRemovableDevice(void);

/**
 * @brief Request to refreshing removable devices
 *
 * @return true if operation is successfull, false otherwise.
 */
frost_bool ICE_RefreshDevices(void);

/**
 * @brief Copy files NVRAM files To the first removable drive
 *
 * @return true if operation is successfull, false otherwise.
 */
frost_bool ICE_CopyFilesToRemovableDevice(void);

frost_bool ICE_EnableRemovableDevice(frost_bool enable);

void ICE_RemovableDeviceEnableDLNACheck(frost_bool enable);


#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_REMOVABLE_DEVICE_H_INCLUDED*/

