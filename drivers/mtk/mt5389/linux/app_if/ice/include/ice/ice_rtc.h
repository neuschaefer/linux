/**
 * Copyright 2002-3 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  Interface to the real-time clock.
 *
 * This file describes the ICE interface to the real-time clock.
 *
 * The clock keeps track of the number of elapsed seconds, and its range
 * is limited to 32-bits.  No starting time is specified, as this is the
 * responsibility of the client code.
 */

#ifndef CABOT_ICE_ICE_RTC_H_INCLUDED
#define CABOT_ICE_ICE_RTC_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Initialise the real-time clock interface.
 *
 * This call must be made prior to setting or reading the RTC.
 */
void ICE_InitRTC(void);

/**
 * @brief Set the real-time clock.
 *
 * The clock is set to the given number of seconds, and updated in real-time.
 *
 * @param time The number of elapsed seconds.
 */
void ICE_SetRTC(frost_uint32 time);

/**
 * @brief Query the real-time clock.
 *
 * @returns The current value of the clock in elapsed seconds.
 */
frost_uint32 ICE_QueryRTC(void);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_RTC_H_INCLUDED*/
