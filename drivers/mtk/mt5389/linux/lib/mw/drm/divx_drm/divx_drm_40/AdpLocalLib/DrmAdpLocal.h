/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_40/AdpLocalLib/DrmAdpLocal.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

/*
These function are the actual implementation to read and write from
the persistent memory.

Production players need to implement these functions according to 
how the memory is stored in the product.
*/

#ifndef DRMADPLOCAL_H_INCLUDE
#define DRMADPLOCAL_H_INCLUDE

#ifndef __KERNEL__

#include "AdpLib/DrmAdpTypes.h"

#else

#include "../AdpLib/DrmAdpTypes.h"

#endif

/* Returns. */
#define ADP_LOCAL_SUCCESS                   0
#define DRM_ERROR_READING_MEMORY			12			
#define DRM_ERROR_WRITING_MEMORY			13


#ifdef __cplusplus
extern "C" {
#endif

uint32_t localClock(void);
uint32_t localClockTicksPerSec(void);
void localSetModelId(uint8_t *modelId);
uint8_t localLoadDrmMemory(uint8_t *memory);
uint8_t localSaveDrmMemory(uint8_t *memory);

#ifdef __cplusplus
};
#endif

#endif
/* DRMADPLOCAL_H_INCLUDE */
