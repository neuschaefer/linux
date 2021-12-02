/**
 * Copyright 2002 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for getting Transport Packet payload data
 *
 * This interface is used by Eclipse to retrieve vbi line data.
 * This API is used to retrieve teletext data, 
 * EBU Teletext data.  If Cabot's Teletext decoder 
 * (including Teletext subtitle decoder) are not 
 * integrated, then a stub implementation of this API may be used.
 */

#ifndef CABOT_ICE_ICE_VBI_COLLECTOR_H_INCLUDED
#define CABOT_ICE_ICE_VBI_COLLECTOR_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_device_number.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque type used to identify an open VBI collector device.
 */
typedef struct ICE_VBI_CollectorDeviceHandle ICE_VBI_CollectorDeviceHandle;

/**
 * @brief Type of data sent to Eclipse via ICE_VBI_CollectorReceivedCallback. 
 */
typedef enum ICE_VBIDataType
{
    ice_teletext_data,
    ice_vps_data,
    ice_wss_data
}ICE_VBIDataType;


/**
 * @brief Prototype for the callback function.
 *
 * The ICE VBI data retrieving implementation should
 * call this function whenever data is received
 * in a call to ICE_VBI_CollectorStart().
 *
 * @param user_data The user_data value specified in the call to
 * ICE_VBI_CollectorStart().
 *
 * @param pdata A pointer to a buffer owned by ICE containing some
 * VBI line data from the VBI slicer. A typical implementation is expected
 * to call this function for each field VBI data lines received.
 * pdata should be 42 bytes data starting from magazine number, it does not include
 * the framing code (0xE4)
 *
 * @param length The number of bytes in the <code>pdata</code> buffer.
 */
typedef void (*ICE_VBI_CollectorReceivedCallback)(void *user_data,
                                                  const frost_uint8 *pdata,
                                                  frost_uint length,
                                                  ICE_VBIDataType data_type);

/**
 * @brief Initialise VBI collector device driver.
 *
 * Called before any other VBI collect functions are called.
 *
 * @return #frost_true on success, #frost_false on failure. 
 *
 * @note If called when already initialised, return #frost_true.
 */
frost_bool ICE_VBI_CollectorInit(void);


/**
 * @brief Open a VBI collector device handle.
 *
 * @param source_device_number Identifies the source of the data for the VBI collector device.
 *
 * @return A handle to a TPData device if successful; otherwise 0.
 *
 * @note If called for a device that has already been opened, return the
 * existing handle for the device.
 */
ICE_VBI_CollectorDeviceHandle* ICE_VBI_CollectorDeviceOpen(ICE_DeviceSource source_device_number);


/**
 * @brief Close a VBI collector device handle.
 *
 * @param device_handle VBI collector device handle returned by a previous call to
 * ICE_VBI_CollectorDeviceOpen(). If passed a null handle, this function should do
 * nothing and return #frost_true.
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note If ICE_VBI_CollectorDeviceOpen() was called multiple times for a particular 
 * device, this function should do nothing and return #frost_true until it is 
 * called the same number of times for that device.
 */
frost_bool ICE_VBI_CollectorDeviceClose(ICE_VBI_CollectorDeviceHandle* device_handle);


/**
 * @brief Start collecting data.
 *
 * Requests that TP payload data from the specified PID should be
 * retrieved using the specified device, and passed up to Eclipse via
 * the callback function specified.
 *
 * @param handle A VBI collector device handle returned by a previous call to
 * ICE_VBI_CollectorDeviceOpen().
 *
 * @param VBI_CollectorReceivedCallback A function which should be called
 * whenever vbi lines data is received.  See notes in
 * ICE_VBI_CollectorReceivedCallback().
 *
 * @param user_data Should be passed, unchanged, back to Eclipse when
 * calling <code>VBI_CollectorReceivedCallback</code>.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_VBI_CollectorStart(ICE_VBI_CollectorDeviceHandle* handle,
                            ICE_VBI_CollectorReceivedCallback VBI_CollectorReceivedCallback,
                            void * user_data);


/**
 * @brief Stop collecting data.
 *
 * Requests that no more calls to the <code>VBI_CollectorReceivedCallback</code>
 *
 * @param handle TPData client handle returned by a previous call to
 * ICE_VBI_CollectorDeviceOpen().
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_VBI_CollectorStop(ICE_VBI_CollectorDeviceHandle* handle);

/**
 * @brief analgue txt avaliabllity.
 *
 * @return #frost_true if txt available,otherwise #frost_false.
 */


frost_bool ICE_VBI_TxtAvailable(void);


#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_VBI_COLLECTOR_H_INCLUDED*/
