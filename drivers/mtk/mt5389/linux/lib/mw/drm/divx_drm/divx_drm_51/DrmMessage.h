/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/DrmMessage.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

/*
These functions and structures allow representation and manipulation
of the challenge response messages used by the ADP DRM.
*/

#ifndef __DRMMESSAGE_H
#define __DRMMESSAGE_H

#include "DrmTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

void packdrmRegistrationRequest( drmRegistrationRequest_t request,
                                   drmMessagePacked_t *messagePacked );
void packRegistrationLogic( drmRegistrationRequest_t *request,
                            drmMessagePacked_t *messagePacked,
                            uint8_t direction );
void registrationSecuritySwap( drmMessagePacked_t *messagePacked );
void wrapSecureMessage( drmMessagePacked_t *transformMessage);
void unwrapSecureMessage( drmMessagePacked_t *transformMessage );

#ifdef __cplusplus
};
#endif

#endif
