/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/local/DrmLocal.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

/*
These function are the actual implementation to read and write from
the persistent memory.

Production players need to implement these functions according to 
how the memory is stored in the product.
*/

#ifndef DRMLOCAL_H_INCLUDE
#define DRMLOCAL_H_INCLUDE

#ifndef __KERNEL__

#include "DrmTypes.h"

#else

#include "../DrmTypes.h"

#endif

/* Returns. */
#define DRM_LOCAL_SUCCESS                   0
#define DRM_ERROR_READING_MEMORY			12			
#define DRM_ERROR_WRITING_MEMORY			13

#ifdef __cplusplus
extern "C" {
#endif

uint32_t localClock( void );
uint32_t localClockTicksPerSec( void );

#if DRM_DATALOAD_USE_LHS == 1
uint32_t localGetLocalHardwareSecret( uint8_t *lhs );
#endif

/* These functions must write 3 fragments into known locations into the OS to counter basic replay attacks:
   For devices with secure memory, a single fragment will suffice, but note, all three functions are still called so
   they must be valid but a NOOP. */
uint32_t localLoadDrmMemoryFragment1( uint8_t *frag,
                                      uint32_t fragLength );

#if DRM_OTHER_SECURE_PLATFORM != 1
uint32_t localLoadDrmMemoryFragment2( uint8_t *frag,
                                      uint32_t fragLength );
uint32_t localLoadDrmMemoryFragment3( uint8_t *frag,
                                      uint32_t fragLength );
#endif

uint32_t localSaveDrmMemoryFragment1( uint8_t *frag,
                                      uint32_t fragLength );

#if DRM_OTHER_SECURE_PLATFORM != 1
uint32_t localSaveDrmMemoryFragment2( uint8_t *frag,
                                      uint32_t fragLength );
uint32_t localSaveDrmMemoryFragment3( uint8_t *frag,
                                      uint32_t fragLength );
#endif

/* The following functions are local accessors to data which is required to assemble an real time key (i.e. hardware key)
   which binds the firmware to the device.

   Depending on the platform, different local functions will be used. */
  
 
/*****************************************************************************************************************
 *****************************************************************************************************************
 *
 *   WINDOWS MOBILE DEVICES
 *
 *****************************************************************************************************************
 *****************************************************************************************************************/
#if DRM_MOBILE_WINDOWS_PLATFORM == 1

/** @brief @b localGetSerial accesses ESN or MEID identification number

This expects a dataLength of either 4 bytes(ESN) or 7 bytes (MEID). This expects the serial from
the lineGeneralInfo. 

@return uint32_t
@retval 0 - success
@retval 1 - failure
@param[in] serial - pointer to a valid sized buffer of 7 bytes.
@param[out] serialLength - pointer to how many bytes were returned in the esnormeid parameter.

*/
uint32_t localGetSerial( uint8_t* serial,
                         uint32_t* serialLength ); 

/** @brief @b localGetRevision accesses the revision info from the Windows Mobile Device

This expects revision from the lineGeneralInfo.

@return uint32_t
@retval 0 - success
@retval 1 - failure
@param[in] revision - pointer to a valid sized buffer for the revision.
@param[out] revisionLength - pointer to how many bytes were returned in the esnormeid parameter.

*/
uint32_t localGetRevision( uint8_t* revision,
                           uint32_t* revisionLength );


/** @brief @b localGetManufacturer accesses the manufacturer string from the Windows Mobile Device

This expects manufacturer from the lineGeneralInfo.

@return uint32_t
@retval 0 - success
@retval 1 - failure
@param[in] manufacturer - pointer to a valid sized buffer for the revision.
@param[out] manufacturerLength - pointer to how many bytes were returned in the esnormeid parameter.

*/
uint32_t localGetManufacturer( uint8_t* manufacturer,
                               uint32_t* manufacturerLength );

#endif

/*****************************************************************************************************************
 *****************************************************************************************************************
 *
 *   SYMBIAN MOBILE DEVICES
 *
 *****************************************************************************************************************
 *****************************************************************************************************************/
#if DRM_MOBILE_SYMBIAN_PLATFORM == 1

/** @brief @b localGetIMEI accesses IMEI identification number

This expects a dataLength of either 15 or 16 bytes depending if this is a IMEI or IMEISV number.
Each byte represents a digit in the IMSI

@return uint32_t
@retval 0 - success
@retval 1 - failure
@param[in] imei - pointer to a valid sized buffer of 16 bytes.
@param[out] imeiLength - pointer to how many bytes were returned in the imei parameter.

*/
uint32_t localGetIMEI( uint8_t* imei,
                       uint32_t* imeiLength ); 

/** @brief @b localGetMachineId accesses machine ID

This expects the machine ID from the Symbian device.

@return uint32_t
@retval 0 - success
@retval 1 - failure
@param[in] machineId - pointer to a valid sized buffer of at least 4 bytes. Each byte represents a digit in the symbian machine ID
@param[out] machineIdLength - pointer to how many bytes were returned in the machineId parameter.  Maximum must be 4 bytes.

*/
uint32_t localGetMachineId( uint8_t* machineId,
                            uint32_t* machineIdLength ); 

#endif

/*****************************************************************************************************************
 *****************************************************************************************************************
 *
 *   LINUX MOBILE DEVICES
 *
 *****************************************************************************************************************
 *****************************************************************************************************************/
#if DRM_MOBILE_LINUX_PLATFORM == 1

/** @brief @b localGetUniqueUserData accesses IMEI identification number

This IMEI identification number 

@return uint32_t
@retval 0 - success
@retval 1 - failure
@param[in] userData - pointer to a valid sized buffer of 16 bytes.
@param[out] userDataLength - pointer to how many bytes were returned in the imei parameter.

*/
uint32_t localGetUniqueUserData( uint8_t* userData,
                                 uint32_t* userDataLength );

/** @brief @b localGetUniqueDeviceData must retrieve a device data for the linux system



@return uint32_t
@retval 0 - success
@retval 1 - failure
@param[in] deviceData - pointer to a valid sized buffer of 16 bytes.
@param[out] deviceDataLength - pointer to how many bytes were returned in the imei parameter.

*/
uint32_t localGetUniqueDeviceData( uint8_t* deviceData,
                                   uint32_t* deviceDataLength ); 
#endif


/*****************************************************************************************************************
 *****************************************************************************************************************
 *
 *   WINDOWS PLATFORM
 *
 *****************************************************************************************************************
 *****************************************************************************************************************/
#if DRM_DESKTOP_WINDOWS_PLATFORM == 1 || DRM_DESKTOP_LINUX_PLATFORM == 1

/** @brief @b localGetMacAddress accesses MAC

This is the MAC address.  The ":" delimeters are optional.  If no MAC address this
should return a zero'ed out MAC address.

@return uint32_t
@retval 0 - success
@retval 1 - failure
@param[in] mac - pointer to a valid sized buffer of 16 bytes.
@param[out] macLength - pointer to how many bytes were returned in the mac parameter.

*/
uint32_t localGetMacAddress( uint8_t* mac,
                             uint32_t* macLength );

/** @brief @b localGetProcessorString accesses the processor string value

This can be any length string that describes the processor type.

@return uint32_t
@retval 0 - success
@retval 1 - failure
@param[in] processorString - pointer to a valid sized buffer
@param[out] processorStringLength - pointer to how many bytes are in the processor string.

*/
uint32_t localGetProcessorString( uint8_t* processorString,
                                  uint32_t* processorStringLength );
                                  
/** @brief @b localGetBootDriveName accesses the name of the boot drive

This can be any length string that describes the processor type.

@return uint32_t
@retval 0 - success
@retval 1 - failure
@param[in] bootDriveName - pointer to a valid sized buffer
@param[out] bootDriveNameLength - pointer to how many bytes are in the hard drive name.

*/
uint32_t localGetBootDriveName( uint8_t* bootDriveName,
                                uint32_t* bootDriveNameLength ); 
#endif

#ifdef __cplusplus
};
#endif

#endif
/* DRMLOCAL_H_INCLUDE */
