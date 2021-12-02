/*
   // $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/HeaderStructs2Mem.h#1 $
   // Copyright (c) 2006 DivX, Inc. http://www.divx.com/corporate
   // All rights reserved.
   //
   // This software is the confidential and proprietary information of DivX
   // Inc. ("Confidential Information").  You shall not disclose such Confidential
   // Information and shall use it only in accordance with the terms of the license
   // agreement you entered into with DivX, Inc.
 */
#ifndef __STRUCTS2MEM_H
#define __STRUCTS2MEM_H

#ifndef __KERNEL__

#include "DivXAesDecrypt.h"
#include "DivXAesEncrypt.h"
#include "DrmTypes.h"

#else

#include "crypto/DivXAesDecrypt.h"
#include "crypto/DivXAesEncrypt.h"
#include "DrmTypes.h"

#endif

#if defined __cplusplus
extern "C"
{
#endif
                            
int32_t activateRecord2Mem( drmActivateRecord_t* activationRecord, 
                            uint8_t** mem,
                            aesKeyHandle keyHandle );
int32_t frameKeys2Mem( const drmFrameKeys_t* frameKeys,
                       uint8_t** mem,
                       aesKeyHandle keyHandle );
int32_t targetHeader2Mem( const drmTargetHeader_t* targetHeader,
                          uint8_t** mem,
                          aesKeyHandle keyHandle );
int32_t drmMemory2Mem( drmMemory_t * drmMemory,
                       uint8_t * * mem,
                       aesKeyHandle keyHandle );

void unpackSlots(uint8_t* packMem, drmMemory_t* dmem);
void packSlots(drmMemory_t* dmem, uint8_t* mem);

#if defined __cplusplus
}
#endif

#endif

