/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file    ice_dsmcc_nvm_caching.h
 * @brief   This function 
 *
 * This file defines the ICE interface needed for NVM file caching.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 */

#ifndef CABOT_ICE_ICE_DSMCC_NVM_CACHING_H_INCLUDED
#define CABOT_ICE_ICE_DSMCC_NVM_CACHING_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The value returned by this function is specified by the manufacturer
 * and the broadcaster. It may reflect the receiver type or the memory model
 * or some other aspect of the receiver.
 *
 * @return Returns platform specific value or zero.
 */
frost_uint8 ICE_DsmccNvmCachingReceiverProfile(void);

/**
 * @brief Determines the amount of persistent storage available for the 
 * storage of files carried in the carousel.
 *
 * This feature is required by the South Africa and Australia MHEG-5 profiles.
 *
 * For South Africa the value returned could be several hundred megabytes. For
 * Australia the value returned is more closely aligned with the size of a typical
 * Australian broadcast carousel - approx 2 megabytes.
 *
 * &return The number of bytes available for the caching of files.
 */
frost_uint32 ICE_DsmccNvmCachingMaxSize(void);


#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_DSMCC_NVM_CACHING_H_INCLUDED*/
