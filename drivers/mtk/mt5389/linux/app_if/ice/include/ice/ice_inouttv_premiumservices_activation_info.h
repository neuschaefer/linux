/**
 * Copyright 2008 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  Interface to InOutTV Advertisement Data Reading functionality.
 *
 * This interface is only required for InOutTV specific projects (like NouCamp).
 */

#ifndef CABOT_ICE_ICE_INOUTTV_PREMIUMSERVICES_ACTIVATION_INFO_H_INCLUDED
#define CABOT_ICE_ICE_INOUTTV_PREMIUMSERVICES_ACTIVATION_INFO_H_INCLUDED

#include "frost/frost_basictypes.h"


#ifdef __cplusplus
extern "C" {
#endif

frost_bool ICE_GetPremiumServiceActivationInfo(frost_uint8 *data, frost_uint8 size);
frost_bool ICE_SetPremiumServiceActivationInfo(frost_uint8 *data, frost_uint8 size);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_INOUTTV_PREMIUMSERVICES_ACTIVATION_INFO_H_INCLUDED */
