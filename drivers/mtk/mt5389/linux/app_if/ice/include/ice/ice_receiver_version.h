/**
 * Copyright 2003 Cabot Communications.  All rights reserved.
 * @file
 * @brief ICE receiver version interface
 *
 * This file defines the interface needed by the Cabot MHEG-5 Engine
 * to obtain receiver identifiers from the underlying receiver software.
 *
 * @note An MHEG-5 application can query a receiver to determine if a
 * given input identifier matches one of the receiver's internal
 * identifiers.  The result of this query allows application
 * authors to provide different paths through an application for
 * different receivers, though typically such branching is kept to a
 * minimum.
 *
 * Reference: Digital Terrestrial Television, MHEG-5 Specification
 * (aka UK Profile) Section 3.4.1, GetEngineSupport, feature string
 * UKEngineProfile.
 */

#ifndef CABOT_ICE_ICE_RECEIVER_VERSION_H_INCLUDED
#define CABOT_ICE_ICE_RECEIVER_VERSION_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The number of characters in the principal version identifier
 */
#define ICE_PRINCIPAL_IDENTIFIER_LENGTH 9

/**
 * @note The principal version identifier:
 *
 *     "uniquely identifies the receiver via its make, model and
 *     version number.  This shall be of the form "mmmcccvvv", where
 *     "mmm" uniquely identifies the manufacturer, "ccc" is a unique
 *     model code and "vvv" is the principal version of the complete
 *     receiver software build.  The subfields "mmm", "ccc" and "vvv"
 *     shall be encoded as 3 octets with values in the range 0x21 to
 *     0x7E inclusive."  (UK Profile, Version 1.06, Section 3.4.1.3)
 */

/**
 * @brief Obtain the number of receiver identifiers.
 *
 * This function enables the receiver to declare the number of version
 * identifiers it responds to. This number must be at least one, since
 * all receivers should have a principal identifier.
 *
 * @note The receiver does not need to provide identifiers for the
 * DSM-CC and MHEG engine stacks, as these are handled internally.
 *
 * @return The number of identifiers.
 */
frost_uint ICE_GetReceiverIdentifierCount(void);

/**
 * @brief Obtain the length of a version identifier
 *
 * @note The identifier_index will be in the range
 * [0, ICE_GetReceiverIdentifierCount() - 1]
 * @note If the identifier_index is zero, this function should return
 * #ICE_PRINCIPAL_IDENTIFIER_LENGTH
 *
 * @param identifier_index The index of the requested identifier.
 *
 * @return The length of the identifer for this particular index.
 */
frost_uint ICE_GetReceiverIdentifierLength(frost_uint identifier_index);

/**
 * @brief Obtain a receiver identifier.
 *
 * @note The identifier_index will be in the range
 * [0, ICE_GetReceiverIdentifierCount() - 1]
 * @note If the <code>identifier_index</code> is zero, this function should
 * respond with the principal version identifier.
 *
 * @param identifier_index The index of the requested identifier.
 * @param identifier A pointer to
 *        <code>ICE_GetReceiverIdentifierLength</code>(identifier_index)
 *        unsigned chars, all of which will be set by this function call.
 *
 * @return #frost_true on success, otherwise #frost_false.
 */
frost_bool ICE_GetReceiverIdentifier(frost_uint identifier_index,
                                     unsigned char* identifier);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_RECEIVER_VERSION_H_INCLUDED*/
