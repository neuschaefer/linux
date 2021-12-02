/**
 * Copyright 2008 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  Interface to InOutTV EPG descrambling functionality.
 *
 * This interface is only required for InOutTV specific projects (like NouCamp).
 */

#ifndef CABOT_ICE_ICE_INOUTTVEPGDESCRAMBLER_H_INCLUDED
#define CABOT_ICE_ICE_INOUTTVEPGDESCRAMBLER_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_device_number.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Start descrambling over given pid with fixed keys.
 *
 * @param pid The pid that is used for downloading scrambled EIT data.
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note Before this call a filter must be set to param pid.
 */
frost_bool ICE_InOutTVEPGDescramblerStart(frost_uint pid);

/**
 * @brief Close a connection to the descrambling channel.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_InOutTVEPGDescramblerStop(void);


#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_INOUTTVEPGDESCRAMBLER_H_INCLUDED */
