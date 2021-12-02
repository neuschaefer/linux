/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  Interface to the South Africa Unique Id 
 *
 * This file describes the access requirements of the south africa
 * unique id.
 */

#ifndef CABOT_ICE_ICE_SOUTH_AFRICA_UNIQUE_ID_H_INCLUDED
#define CABOT_ICE_ICE_SOUTH_AFRICA_UNIQUE_ID_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief The length of the unique id to be held within a South Africa
 * digital television receiver.
 */
#define ICE_SOUTH_AFRICA_UNIQUE_ID_LENGTH 14

/**
 * @brief Get the unique id held by the reciever.
 *
 * @param result The unique id held by the receiver.
 *
 * @return The number of bytes placed in the result array.
 *
 */
frost_uint8 ICE_GetSouthAfricaUniqueId(frost_uint8 result[ICE_SOUTH_AFRICA_UNIQUE_ID_LENGTH]);
 
#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_SOUTH_AFRICA_UNIQUE_ID_H_INCLUDED*/
