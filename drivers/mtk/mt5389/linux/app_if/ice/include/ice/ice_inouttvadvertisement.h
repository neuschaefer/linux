/**
 * Copyright 2008 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  Interface to InOutTV Advertisement Data Reading functionality.
 *
 * This interface is only required for InOutTV specific projects (like NouCamp).
 */

#ifndef CABOT_ICE_ICE_INOUTTVADVERTISEMENT_H_INCLUDED
#define CABOT_ICE_ICE_INOUTTVADVERTISEMENT_H_INCLUDED

#include "frost/frost_basictypes.h"


#ifdef __cplusplus
extern "C" {
#endif

    frost_uint32 ICE_GetAdvertisementDataSize (void);
    frost_bool ICE_GetAdvertisementData (char *data_p);
    frost_bool ICE_AdvertisementVersion(unsigned char * data_p);
    frost_bool ICE_WriteAdvertisementVersionToFlash(unsigned char * data_to_write);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_INOUTTVADVERTISEMENT_H_INCLUDED */
