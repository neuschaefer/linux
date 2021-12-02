/**
 * Copyright 2007 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  Interface to the MAC address or other unique host ID.
 *
 * This file describes the ICE interface to a Universally Unique ID for the
 * system, typically an ethernet MAC address.
 */

#ifndef CABOT_ICE_ICE_UUID_H_INCLUDED
#define CABOT_ICE_ICE_UUID_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Get the MAC address or other unique Node ID for this individual STB.
 *
 * The 48-bit result is stored into an array of 6 bytes, most significant byte
 * first.
 *
 * Typically the result is a MAC address.
 *
 * If no MAC address is available, the result must be a high-quality random
 * number generated at manufacture time, and the least significant bit of the
 * first byte in the result must be set to 1
 * (i.e. <code>result[0] |= 1;</code>).
 *
 * The Node ID returned by this function must never change, it must persist
 * across reboots for the life of the STB.  The Node ID must be unique; very
 * bad things will happen if this STB is connected to the same network as
 * another device with the same Node ID.
 *
 * @param result a pointer to a 6-byte array to hold the Node ID.
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_GetNodeIdForUUID(frost_uint8 result[]);
 
#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_UUID_H_INCLUDED*/
