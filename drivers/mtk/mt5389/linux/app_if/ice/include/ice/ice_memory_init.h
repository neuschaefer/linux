/**
 * Copyright 2002-3 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Initialisation needed for frost_memory.h
 *
 * All functions in this interface are implemented in STB-specific code and
 * called by Cabot code.
 */

#ifndef CABOT_ICE_ICE_MEMORY_INIT_H_INCLUDED
#define CABOT_ICE_ICE_MEMORY_INIT_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create all memory partitions / areas required by the Set Top and its
 * clients.
 *
 * @note <b>This function must be called prior to any allocation of memory</b>.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_InitialiseSystemMemory(void);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_MEMORY_INIT_H_INCLUDED*/
