/**
 * Copyright 2008 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief ICE interface for loading profile data. 
 *
 */

#ifndef CABOT_ICE_ICE_PROFILE_H_INCLUDED
#define CABOT_ICE_ICE_PROFILE_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Load the profile data with the specified name from nvram.
 * 
 * @param name Name of the profile data to be loaded.
 * 
 * @return Specified profile data.
 * 
 * @note If USE_PROFILE_MANAGER is not enabled this function
 *       need not be implemented.
 */
frost_uint8* ICE_LoadProfileData(const char* name);

#ifdef __cplusplus
}
#endif

#endif /*CABOT_ICE_ICE_PROFILE_H_INCLUDED*/
