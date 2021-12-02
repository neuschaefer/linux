/**
 * Copyright 2002-3 Cabot Communications.  All rights reserved.
 *
 * @file
 *
 * @brief Cabot's interface from the Eclipse system to initialise platform
 * specific debug output.
 *
 */

#ifndef CABOT_ICE_ICE_DEBUG_INIT_H_INCLUDED
#define CABOT_ICE_ICE_DEBUG_INIT_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Perform any required initialisation of the FROST debug I/O system.
 * Once the debug unit has been initialised, subsequent calls to 
 * this function should return #frost_true and take no further action.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_DebugInit(void);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_DEBUG_INIT_H_INCLUDED*/

