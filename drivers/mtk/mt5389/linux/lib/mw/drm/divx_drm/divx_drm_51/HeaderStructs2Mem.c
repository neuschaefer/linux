/*
   // $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/HeaderStructs2Mem.c#1 $
   // Copyright (c) 2006 DivX, Inc. http://www.divx.com/corporate
   // All rights reserved.
   //
   // This software is the confidential and proprietary information of DivX
   // Inc. ("Confidential Information").  You shall not disclose such Confidential
   // Information and shall use it only in accordance with the terms of the license
   // agreement you entered into with DivX, Inc.
 */

#ifndef __KERNEL__

#include "HeaderErrors.h"
#include "HeaderMacros.h"
#include "HeaderStructs2Mem.h"

//#include <memory.h> Using string.h instead since gcc-fm for HSM doens't have memory.h
#ifdef _MTK_BUILD_
#include "libc/memory.h"
#include "libc/string.h"
#else
#include <string.h>
#endif

#else

#include "HeaderErrors.h"
#include "HeaderMacros.h"
#include "HeaderStructs2Mem.h"

#endif

int32_t activateRecord2Mem( drmActivateRecord_t* activationRecord, 
                            uint8_t** mem,
                            aesKeyHandle keyHandle )

{
    uint8_t * localMem = *mem;

    bytes2mem( activationRecord->memoryGuard, 
               localMem,
               sizeof(activationRecord->memoryGuard));
    bytes2mem( activationRecord->modelId, 
               localMem,
               sizeof(activationRecord->modelId));
    bytes2mem( activationRecord->userKey,
               localMem,       
               sizeof(activationRecord->userKey));
    bytes2mem( activationRecord->explicitGuard, 
               localMem,       
               sizeof(activationRecord->explicitGuard));

     *mem = localMem; /* point memory to last position */

    return DRM_ERROR_NONE;
}


int32_t frameKeys2Mem( const drmFrameKeys_t * frameKeys,
                       uint8_t * * mem,
                       aesKeyHandle keyHandle )
{
    uint8_t * localMem = *mem;
    uint8_t * startMem = *mem;
    uint32_t length = 0;
    uint8_t scratch[sizeof(drmFrameKeys_t)];

    /* ENCRYPT START */
    startMem = localMem;
    bytes2mem( frameKeys->frameKeys,
               localMem,
               sizeof(frameKeys->frameKeys) );
    /* ENCRYPT END */

    length = (uint32_t)( localMem - startMem );
    if ( ( length % 16 ) != 0 )
    {
        return DRM_FAILED_CRYPTO;
    }
    
    if ( DivXAesECBEncrypt( keyHandle,
                            startMem,
                            length,
                            scratch,
                            &length ) != 0 )
    {
        return DRM_FAILED_CRYPTO;
    }
    
    /* Move out */
    memcpy( startMem,
            scratch,
            length );

    *mem = localMem; /* point memory to last position */

    return DRM_ERROR_NONE;
}

int32_t targetHeader2Mem( const drmTargetHeader_t * targetHeader,
                          uint8_t * * mem,
                          aesKeyHandle keyHandle )
{
    uint8_t * localMem = *mem;
    /* uint8_t * startMem = *mem; */
    /* uint32_t length = 0; */

    /* Serialize */ 
    int162mem( targetHeader->drmMode,
               localMem);
    bytes2mem( targetHeader->userId,
               localMem,
               sizeof(targetHeader->userId) );
    int82mem( targetHeader->optionFlags,
               localMem);
    int162mem( targetHeader->rentalRecord.useLimitId,
               localMem);
    bytes2mem( targetHeader->rentalRecord.serialNumber,
               localMem,
               sizeof(targetHeader->rentalRecord.serialNumber) );
    int82mem( targetHeader->rentalRecord.slotNumber,
               localMem);
    bytes2mem( targetHeader->rentalRecord.reserved,
               localMem,
               sizeof(targetHeader->rentalRecord.reserved) );
    bytes2mem( targetHeader->sessionKey,
               localMem,
               sizeof(targetHeader->sessionKey) );
    bytes2mem( targetHeader->activateRecord.memoryGuard,
               localMem,
               sizeof(targetHeader->activateRecord.memoryGuard) );
    bytes2mem( targetHeader->activateRecord.modelId,
               localMem,
               sizeof(targetHeader->activateRecord.modelId) );
    bytes2mem( targetHeader->activateRecord.userKey,
               localMem,
               sizeof(targetHeader->activateRecord.userKey) );
    bytes2mem( targetHeader->activateRecord.explicitGuard,
               localMem,
               sizeof(targetHeader->activateRecord.explicitGuard) );
    int82mem( targetHeader->outputProtectionFlags, 
                localMem);
    int82mem( targetHeader->protectedAudioOffset, 
                localMem);
    int82mem( targetHeader->protectedAudioCryptoSize, 
                localMem);
    int82mem( targetHeader->frameKeyCount, 
                localMem);
    int162mem( targetHeader->drmSubMode, 
                localMem);
    int82mem( targetHeader->guardExt.memoryGuardExt, 
                localMem);
    int82mem( targetHeader->guardExt.explicitGuardExt, 
                localMem);

#if 0
    length = (uint32_t)(localMem - startMem);
    if ( ( length % 16 ) != 0 )
    {
        return DRM_FAILED_CRYPTO;
    }
    
    if ( DivXAesECBEncrypt( keyHandle,
                            startMem,
                            length,
                            scratch,
                            &length ) != 0 )
    {
        return DRM_FAILED_CRYPTO;
    }
    
    /* Move out */
    memcpy( startMem,
            scratch,
            length );
#endif
    *mem = localMem; /* point memory to last position */

    return DRM_ERROR_NONE;
}


void packSlots(drmMemory_t* dmem, uint8_t* mem)
{
    /* uint32_t i = 0; */    uint32_t j = 0;
    for (j = 0; j < TOTAL_PLAY_SLOTS; j++)
    {
       *mem = dmem->slots[j].serialNumber[0];  mem+=1;
       *mem = dmem->slots[j].serialNumber[1];  mem+=1;
    }

    *mem = (dmem->slots[0].counter & 0x3F) |
                          ((dmem->slots[1].counter & 0x03) << 6) ;   mem+=1;

    *mem = ((dmem->slots[1].counter & 0x3C) >> 2) |
                            ((dmem->slots[2].counter & 0x0F) << 4) ; mem+=1;
    
    *mem = ((dmem->slots[2].counter & 0x30) >> 4) |   
                         ((dmem->slots[3].counter & 0x3F) << 2) ;    mem+=1;

    *mem = ((dmem->slots[4].counter & 0x3F)) |   
                         ((dmem->slots[5].counter & 0x03) << 6) ;    mem+=1;
    
    *mem = ((dmem->slots[5].counter & 0x3C) >> 2) |   
                         ((dmem->slots[6].counter & 0x0F) << 4) ;    mem+=1;

    *mem = ((dmem->slots[6].counter & 0x30) >> 4) |
                           ((dmem->slots[7].counter & 0x3F) << 2) ;  mem+=1;
}

void unpackSlots(uint8_t* packMem, drmMemory_t* dmem)
{
    uint32_t i = 0; /* uint32_t j = 0; */
    for (i = 0; i < 8; i++)
    {
        dmem->slots[i].serialNumber[0] = *packMem ; packMem++;
        dmem->slots[i].serialNumber[1] = *packMem ;  packMem++;
    }

    dmem->slots[0].counter = (*packMem & 0x3F);

    dmem->slots[1].counter = ((*packMem & 0xC0) >> 6) |
                                   ((*(packMem+1) & 0x0F) << 2);  packMem++;

    dmem->slots[2].counter = ((*packMem & 0xF0) >> 4) |
                                   ((*(packMem+1) & 0x03) << 4);  packMem++;

    dmem->slots[3].counter = ((*packMem & 0xFC) >> 2);  packMem++;

    dmem->slots[4].counter = (*packMem & 0x3F) ;

    dmem->slots[5].counter = ((*packMem & 0xC0) >> 6) | 
                                 ((*(packMem+1) & 0x0F) << 2);  packMem++;

    dmem->slots[6].counter = ((*packMem & 0xF0) >> 4) | 
                                 ((*(packMem+1) & 0x03) << 4);  packMem++;

    dmem->slots[7].counter = ((*packMem & 0xFC) >> 2);
}

int32_t drmMemory2Mem( drmMemory_t * drmMemory,
                       uint8_t * * mem,
                       aesKeyHandle keyHandle )
{
    uint8_t * localMem = *mem;
    /* uint8_t * startMem = *mem; */
    /* uint32_t length = 0; */
    /* uint32_t i = 0; */
    
    /* Serialize */ 
#if DRM_OTHER_SECURE_PLATFORM == 0
    bytes2mem( drmMemory->prefixPad,
               localMem,
               sizeof(drmMemory->prefixPad) );
    bytes2mem( drmMemory->randomPad1,
               localMem,
               sizeof(drmMemory->randomPad1) );
#endif          
    bytes2mem( drmMemory->owner.guard,
               localMem,
               sizeof(drmMemory->owner.guard) );
#if DRM_OTHER_SECURE_PLATFORM == 0
    int82mem( drmMemory->owner.randomPad1,
               localMem );
#endif
    bytes2mem( drmMemory->owner.key,
               localMem,
               sizeof(drmMemory->owner.key) );
#if DRM_OTHER_SECURE_PLATFORM == 0
    int82mem( drmMemory->owner.randomPad2,
               localMem );
#endif
    bytes2mem( drmMemory->owner.userId,
               localMem,
               sizeof(drmMemory->owner.userId) );
#if DRM_OTHER_SECURE_PLATFORM == 0
    int82mem( drmMemory->owner.randomPad3,
               localMem );
#endif

#if DRM_OTHER_SECURE_PLATFORM == 0
    for ( i = 0; i < TOTAL_PLAY_SLOTS; i++ )
    {
        bytes2mem( drmMemory->slots[i].serialNumber,
                   localMem,
                   sizeof(drmMemory->slots[i].serialNumber) );

        int82mem( drmMemory->slots[i].counter,
                  localMem );

        int82mem( drmMemory->slots[i].randomPad,
                  localMem );
    }
#else
    /*In the 48 byte configuration, we pack the slot counters into 6 bytes*/
    packSlots(drmMemory, localMem);
#endif

#if DRM_OTHER_SECURE_PLATFORM == 0
    int322mem( drmMemory->activationFailureCnt,
               localMem );
    bytes2mem( drmMemory->randomPad2,
               localMem,
               sizeof(drmMemory->randomPad2) );
    bytes2mem( drmMemory->postfixPad,
               localMem,
               sizeof(drmMemory->postfixPad) );
#endif               

    *mem = localMem; /* point memory to last position */

    return DRM_ERROR_NONE;
}

