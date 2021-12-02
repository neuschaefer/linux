/**
 * Copyright 2002-5 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for System Time Clock and PCR
 *
 * The System Time Clock is a clock that is maintained by the receiver.  This
 * clock is recovered from PCR values that are broadcast on the PCR PID.
 * This file defines the ICE interfaces to allow Eclipse to set the PCR PID,
 * and to read the derived System Time Clock values.
 *
 * Hardware that only supports a single MPEG decoder typically only supports
 * a single STC.  Devices which support two simultaneous MPEG video decodes
 * typically support two STCs.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 */

#ifndef CABOT_ICE_ICE_STC_H_INCLUDED
#define CABOT_ICE_ICE_STC_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_device_number.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Opaque type used to identify an open connection to a System Time
 * clock.
 */
typedef struct ICE_StcHandle ICE_StcHandle;


/**
 * @brief Open a connection to the STC device.
 *
 * Attempts to open a connection to the STC device identified and
 * return a handle for subsequent communication with the device.
 *
 * @param device_number The ID of the required STC device.
 *
 * @return Returns a pointer to the handle to the device on success
 *         and a NULL pointer on failure.
 */
ICE_StcHandle* ICE_StcOpen(ICE_DeviceNumber device_number);


/**
 * @brief Close a connection to the STC device.
 *
 * Closes a previously opened connection to the STC device identified.
 *
 * @param handle The handle to the required STC device.  May be NULL,
 *               in which case this function will do nothing.
 */
void ICE_StcClose(ICE_StcHandle* handle);


/**
 * @brief Set the PID to be used for the PCR.
 *
 * Set the PID to be used for extraction of the PCR.
 *
 * Typically this will be called before starting audio or video decoding.
 *
 * @param handle The handle to the required STC device.
 * @param pcr_pid The PCR PID value.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_StcSetPcrPid(ICE_StcHandle* handle, frost_uint pcr_pid);


/**
 * @brief Clears the PID to be used for the PCR.
 *
 * Clears any PID that was being used for extraction of the PCR.
 *
 * Typically this will be called when stopping audio and video - e.g. on
 * a channel change.
 *
 * @param handle The handle to the required STC device.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_StcClearPcrPid(ICE_StcHandle* handle);


/**
 * @brief Get the current decoder time (derived from PCR).
 *
 * Return the 33-bit 90kHz local system time clock derived from the PCR.
 *
 * This is derived from the 33-bit 90kHz "base" PCR value.  The 9-bit
 * "extension" PCR value (for a 27MHz clock) is not returned via this API.
 *
 * @note Eclipse calls this function to synchronise the presentation of
 * subtitles with the A/V.  It will be called, at most, a few times a second.
 * Eclipse currently only uses the low 32 bits of the PCR, so there is
 * currently no need to set the high bit (however, this may be required in
 * future so is recommended).
 *
 * @note If you are not using Cabot subtitles, you may implement this function
 *       to always fail.
 *
 * @param handle The handle to the required STC device.
 * @param result Used to store the result.  Will not be NULL.
 *
 * @return #frost_true on success, #frost_false if no PCR PID is set or no
 *         PCR is present on the PCR PID.
 */
frost_bool ICE_StcGetClock(ICE_StcHandle* handle, frost_uint64s* result);


/**
 * @brief Configure the transport stream source for subsequent calls to
 * ICE_StcSetPcrPid.
 *
 * @param handle The handle to the required STC device.
 * @param source The source of the transport stream, e.g. tuner 0, tuner 1 or
 * hard disk.
 *
 * @note For single tuner platforms with no HDD or support for the
 * ice_ts_player API, this function may be ignored.
 */
void ICE_StcSelectSource(ICE_StcHandle* handle, ICE_DeviceSource source);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_STC_H_INCLUDED*/
