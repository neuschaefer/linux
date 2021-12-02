/**
 * Copyright 2002-2004 Cabot Communications.  All rights reserved.
 * @file
 * @brief ICE Interface for implementing MHEG 1.06 debug messages
 */

#ifndef CABOT_ICE_ICE_MHEG_DEBUG_OUT_H_INCLUDED
#define CABOT_ICE_ICE_MHEG_DEBUG_OUT_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Output a debug message at the request of an MHEG application
 *
 * This interface has been introduced to support new functionality
 * introduced in UK Profile 1.06.  Full support for this function is
 * optional (i.e. a conformant ICE implementation could simply do nothing):
 * however, if implemented, it will assist MHEG-5 authors in developing
 * applications to run on the receiver. Exactly where the debug
 * message appears will depend on the hardware configuration.
 * Typically a set top box may use a serial port.
 *
 * @param message Null terminated string to be output
 *
 * Reference: Digital Terrestrial Television, MHEG-5 Specification
 * (aka UK Profile) Section 3.10.10.2 Debug
 */
void ICE_MHEG_DebugOut(const char * message);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_MHEG_DEBUG_OUT_H_INCLUDED*/
