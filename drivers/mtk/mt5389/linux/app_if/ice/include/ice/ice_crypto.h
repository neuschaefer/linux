/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for Cryptographic support functions
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 */

#ifndef CABOT_ICE_ICE_CRYPTO_H_INCLUDED
#define CABOT_ICE_ICE_CRYPTO_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the current value of a high resolution time source.
 *
 * The units are "ticks"; the length of a "tick" is implementation specific.
 *
 * Usually this will start at a fixed value when the box boots,
 * and just count up until it wraps.
 *
 * This is used by the cryptographic pseudo-random number generator.
 * So it is important to use as high a resolution time source as you can, for
 * example the processor clock cycles since boot.
 *
 * Under Microsoft Windows this is implemented using the 
 * GetSystemTimeAsFileTime API
 *
 * This must not be implemented as just an incrementing static variable
 *
 * @param ticks_out The ICE implementaion should set this to the time
 *                  in "ticks".
 *
 * @return frost_true on success; frost_false on error.  Note that if this
 *         fails then cryptographic features will fail (e.g. CI+ will stop
 *         working).
 */
frost_bool ICE_CryptoGetHighResTime(frost_uint64s *ticks_out);


/**
 * @brief Fill a buffer with entropy (cryptographically random bytes)
 *
 * The random data produced by this function must be far more random than 
 * simply calling rand(). Entropy is usually generated from either a dedicated 
 * hardware random number generator or by combining values from hardware 
 * components (e.g. jitter, latency and seek times)
 *
 * Entropy must be placed in the buffer from its start address up to the length
 * currently available. 
 *
 * There may not be enough entropy at the time of the call to fill the entire 
 * buffer to the length specified. In this situation this function may be 
 * called in a loop until the desired amount of entropy has been acquired, 
 * typically with the length of the buffer decreasing by the return value of 
 * this function for each subsequent call in the loop.
 *
 * If there is no entropy available at the time of the call, a return value of 
 * 0 can be used. This is not a failure and at some point a future call should 
 * return > 0. 
 *
 * Possible failures include:
 *      A hardware fault (e.g. in a hardware based random number generator)
 *      Any fault that will result in this function never returning any entropy
 *      A failure that is known to cause the entropy to no longer be random
 *
 * Typically a minimum of 16 bytes of entropy will be required at startup to
 * seed the pseudo random number generator. Additional entropy may be required 
 * after startup for salt values, initialisation vectors, continued seeding etc.
 *
 * Entropy must be discarded once it is copied to the destination buffer. This
 * implies that the same entropy must not be used in subsequent calls to this 
 * function. In order to fulfil this requirement this function must be thread 
 * safe.
 *
 * Under Microsoft Windows this is implemented using the CryptGenRandom API
 *
 * @param buffer The buffer to fill with entropy (will never be NULL)
 * @param length The length of the buffer in bytes (will always be > 0)
 *
 * @return The amount of entropy placed in the buffer in bytes or -1 if there 
 * is failure. See above for a definition of what constitutes a failure.
 */
frost_int ICE_CryptoGetEntropy(frost_uint8* buffer, frost_int length);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_TIME_H_INCLUDED*/
