/**
 * Copyright 2002 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for getting Transport Packet payload data
 *
 * This interface is used by Eclipse to retrieve transport packet
 * payload data.  This API is used to retrieve DVB subtitle data, 
 * EBU Teletext data, and ECM/EMM data for embedded Conditional Access
 * Systems (eCAS).  If Cabot's DVB subtitle decoder, Teletext decoder 
 * (including Teletext subtitle decoder) and eCAS engines are not 
 * integrated, then a stub implementation of this API may be used.
 */

#ifndef CABOT_ICE_ICE_TPDATA_H_INCLUDED
#define CABOT_ICE_ICE_TPDATA_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_device_number.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque type used to identify an open TPData device.
 */
typedef struct ICE_TPDataDeviceHandle ICE_TPDataDeviceHandle;


/**
 * @brief Opaque type used to identify an open TPData client.
 */
typedef struct ICE_TPDataClientHandle ICE_TPDataClientHandle;

/**
 * @brief Definition of starting purpose of TPData
 *
 *@note  Purpose should be subtitle or teletext
 */

typedef enum ICE_TPDataStartPurpose
{
    /**
     * TPData started for subtitle
     */
    ICE_tpdata_start_subtitle = 0,

    /**
     * TPData started for teletext
     */
    ICE_tpdata_start_teletext = 1
}ICE_TPDataStartPurpose;

/**
 * @brief Prototype for the callback function.
 *
 * The ICE Transport Packet payload retrieving implementation should
 * call this function whenever data is received on the PID specified
 * in a call to ICE_TPDataStart().
 *
 * @param user_data The user_data value specified in the call to
 * ICE_TPDataStart().
 *
 * @param tp_data A pointer to a buffer owned by ICE containing some
 * TS payload data (i.e. with the adaptation field stripped) from the
 * TP stream identified by the PID specified in a call to
 * ICE_TPDataStart().  A typical implementation is expected to call
 * this function for each TS packet received, after extracting the
 * payload.  However, it is often acceptable to call this function
 * passing multiple concatenated TP payloads (even across PES packet
 * boundaries).  In the limit it would be possible to pass up entire
 * PES packets, reassembled from the TP payloads, but that would
 * likely be inefficient in terms of buffering requirements.
 *
 * @param length The number of bytes in the <code>tp_data</code> buffer.
 *
 * @param data_lost Should be set to #frost_true if TS packets were
 * missed between this call and the preceding call.  Eclipse clients
 * use this indication to reset their state and begin searching for an
 * acquisition point.  Normally this should be set to #frost_false.
 *
 * @note The Eclipse Subtitle client will not cope well if it cannot
 * reliably get all subtitling data from the stream.  (This is true of
 * DVB subtitling in general.)
 *
 * @note When the callback returns, the buffer may be reused or freed
 * as required.  Eclipse will have copied the data if it needs to keep
 * it.
 */
typedef void (* ICE_TPDataReceivedCallback)(
    void * user_data,
    const frost_uint8 * tp_data,
    frost_uint length,
    frost_bool data_lost);


/**
 * @brief Initialise TPData device driver.
 *
 * May be called more than once (but only actioned once), before any other
 * TPData functions are called.
 *
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note If called when already initialised, return #frost_true.
 */
frost_bool ICE_TPDataInit(void);


/**
 * @brief Open a TPData device handle.
 *
 * @param device_type Identifies the source of the data for the TPData device.
 *
 * @return A handle to a TPData device if successful; otherwise 0.
 *
 * @note If called for a device that has already been opened, return the
 * existing handle for the device.
 */
ICE_TPDataDeviceHandle * ICE_TPDataDeviceOpen(ICE_DeviceSource device_type);


/**
 * @brief Close a TPData device handle.
 *
 * @param device_handle TPData device handle returned by a previous call to
 * ICE_TPDataDeviceOpen(). If passed a null handle, this function should do
 * nothing and return #frost_true.
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note If ICE_TPDataDeviceOpen() was called multiple times for a particular 
 * device, this function should do nothing and return #frost_true until it is 
 * called the same number of times for that device.
 */
frost_bool ICE_TPDataDeviceClose(ICE_TPDataDeviceHandle * device_handle);


/**
 * @brief Start extracting transport packet payload data.
 *
 * Requests that TP payload data from the specified PID should be
 * retrieved using the specified device, and passed up to Eclipse via
 * the callback function specified.
 *
 * @param handle A TPData device handle returned by a previous call to
 * ICE_TPDataDeviceOpen().
 *
 * @param pid The PID from which to extract the TP data.
 *
 * @param tpDataReceivedCallback A function which should be called
 * whenever transport packet payload data is received.  See notes in
 * ICE_TPDataReceivedCallback().
 *
 * @param user_data Should be passed, unchanged, back to Eclipse when
 * calling <code>tpDataReceivedCallback</code>.
 *
 * @param purpose  Definition of starting purpose of  tpdata. 
 *
 * @return A handle to a TPData device if successful; otherwise 0.
 */
ICE_TPDataClientHandle * ICE_TPDataStart(
    ICE_TPDataDeviceHandle * handle,
    frost_uint pid,
    ICE_TPDataReceivedCallback tpDataReceivedCallback,
    void * user_data,
    ICE_TPDataStartPurpose purpose);


/**
 * @brief Stop extracting TP payload data.
 *
 * Requests that no more calls to the <code>tpDataReceivedCallback</code>
 * should be made for the specified client handle, obtained from an earlier
 * call to ICE_TPDataStart().
 *
 * @note It is important that, on return from this function, no
 * further calls to the callback are made for the specified handle.
 * For example, implementations using a thread to get transport packet
 * data must take care to synchronise with the thread.
 *
 * @param handle TPData client handle returned by a previous call to
 * ICE_TPDataStart().
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TPDataStop(
    ICE_TPDataClientHandle * handle);


#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_TPDATA_H_INCLUDED*/
