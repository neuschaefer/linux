/*
   // $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/HeaderMem2Structs.c#1 $
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
#include "HeaderMem2Structs.h"
#include "HeaderMacros.h"
#include "HeaderStructs2Mem.h"

//#include <memory.h> Using string.h instead since gcc-fm for HSM doens't have memory.h
#include "DrmTypes.h"
#ifdef _MTK_BUILD_
#include "libc/memory.h"
#include "libc/string.h"
#else
#include <string.h>
#endif

#else

#include "HeaderErrors.h"
#include "HeaderMem2Structs.h"
#include "HeaderMacros.h"
#include "HeaderStructs2Mem.h"

#include "DrmTypes.h"

#endif

int32_t mem2FrameKeys( uint8_t** mem,
                       drmFrameKeys_t * frameKeys,
                       aesKeyHandle keyHandle );
int32_t mem2TargetHeader( uint8_t** mem,
                          drmTargetHeader_t* targetHeader,
                          aesKeyHandle keyHandle );

int32_t mem2ActivateRecord( uint8_t** mem,
                          drmActivateRecord_t* activationRecord,
                          aesKeyHandle keyHandle )

{
    uint8_t * localMem = *mem;
    mem2bytes( localMem,
                activationRecord->memoryGuard, 
                sizeof(activationRecord->memoryGuard));
    mem2bytes( localMem,
                activationRecord->modelId, 
                sizeof(activationRecord->modelId));
    mem2bytes( localMem,
                activationRecord->userKey, 
                sizeof(activationRecord->userKey));
    mem2bytes( localMem,
                activationRecord->explicitGuard, 
                sizeof(activationRecord->explicitGuard));


     *mem = localMem; /* point memory to last position */

    return DRM_ERROR_NONE;
}

int32_t mem2VideoDDChunk( uint8_t * * mem,
                          drmVideoDDChunk_t * videoDDChunk,
                          aesKeyHandle keyHandle )
{
    uint8_t * localMem = *mem;
    /* uint32_t length = 0; */

    /* Translate out */
    mem2int16( localMem,
               videoDDChunk->keyIndex );
    mem2int32( localMem,
               videoDDChunk->offset );
    mem2int32( localMem,
               videoDDChunk->size );
    *mem = localMem; /* point memory to last position */

    return DRM_ERROR_NONE;
}

#define MPF_SIZEOF_FRAME_KEYS    (2048)
int32_t mem2FrameKeys( uint8_t * * mem,
                       drmFrameKeys_t * frameKeys,
                       aesKeyHandle keyHandle )
{
    uint8_t * localMem = *mem;
    uint32_t length = 0;
    uint8_t scratch[sizeof(drmFrameKeys_t)];

    /* DECRYPT HERE */
    length = MPF_SIZEOF_FRAME_KEYS;
    if ( ( length % 16 ) != 0 )
    {
        return DRM_FAILED_CRYPTO;
    }
    
    if ( DivXAesECBDecrypt( keyHandle,
                            localMem,
                            length,
                            scratch,
                            &length ) != 0 )
    {
        return DRM_FAILED_CRYPTO;
    }
    
    /* Move out */
    memcpy( localMem,
            scratch,
            length );

    /* Translate out */
    mem2bytes( localMem,
               frameKeys->frameKeys,
               sizeof(frameKeys->frameKeys) );

    *mem = localMem; /* point memory to last position */

    return DRM_ERROR_NONE;
}


#define SIZEOF_TARGET_HEADER    (64)
int32_t mem2TargetHeader( uint8_t * * mem,
                          drmTargetHeader_t* targetHeader,
                          aesKeyHandle keyHandle )
{
    uint8_t * localMem = *mem;
    uint8_t * startMem = *mem;
    uint32_t length = 0;
    uint8_t scratch[sizeof(drmTargetHeader_t)];
    int32_t i = 0;
    
    /* DECRYPT HERE */
    length = SIZEOF_TARGET_HEADER;
    if ( ( length % 16 ) != 0 )
    {
        return DRM_FAILED_CRYPTO;
    }
    
    if ( DivXAesECBDecrypt( keyHandle,
                            startMem,
                            length,
                            scratch,
                            &length ) != 0 )
    {
        return DRM_FAILED_CRYPTO;
    }

	/* Put in xor. */
	for ( i = SIZEOF_TARGET_HEADER - 1; i > 0; i-- )
	{
		scratch[i] = scratch[i] ^ scratch[i-1];
	}
    
    /* Move out */
    memcpy( startMem,
            scratch,
            length );

    /* Serialize */ 
    mem2int16( localMem,
               targetHeader->drmMode );
    mem2bytes( localMem,
               targetHeader->userId,
               sizeof(targetHeader->userId) );
    mem2int8( localMem,
              targetHeader->optionFlags );
    mem2int16( localMem,
               targetHeader->rentalRecord.useLimitId );
    mem2bytes( localMem,
               targetHeader->rentalRecord.serialNumber,
               sizeof(targetHeader->rentalRecord.serialNumber) );
    mem2int8( localMem,
              targetHeader->rentalRecord.slotNumber );
    mem2bytes( localMem,
               targetHeader->rentalRecord.reserved,
               sizeof(targetHeader->rentalRecord.reserved) );
    mem2bytes( localMem,
               targetHeader->sessionKey,
               sizeof(targetHeader->sessionKey) );
    mem2bytes( localMem,
               targetHeader->activateRecord.memoryGuard,
               sizeof(targetHeader->activateRecord.memoryGuard) );
    mem2bytes( localMem,
               targetHeader->activateRecord.modelId,
               sizeof(targetHeader->activateRecord.modelId) );
    mem2bytes( localMem,
               targetHeader->activateRecord.userKey,
               sizeof(targetHeader->activateRecord.userKey) );
    mem2bytes( localMem,
               targetHeader->activateRecord.explicitGuard,
               sizeof(targetHeader->activateRecord.explicitGuard) );
    mem2int8( localMem,
              targetHeader->outputProtectionFlags );
    mem2int8( localMem,
              targetHeader->protectedAudioOffset );
    mem2int8( localMem,
              targetHeader->protectedAudioCryptoSize );
    mem2int8( localMem,
              targetHeader->frameKeyCount );
    mem2int16( localMem,
               targetHeader->drmSubMode );
    mem2int8( localMem,
               targetHeader->guardExt.memoryGuardExt );
    mem2int8( localMem,
               targetHeader->guardExt.explicitGuardExt );
                
    *mem = localMem; /* point memory to last position */

    return DRM_ERROR_NONE;
}

int32_t mem2TransactionHeader( uint8_t** mem,
                               drmTransactionInfoHeader_t* transactionHeader,
                               aesKeyHandle keyHandle )
{
    uint8_t * localMem = *mem;
    /* uint8_t * startMem = *mem; */
    /* uint32_t length = 0; */

    /* Serialize */ 
    mem2bytes( localMem,
               transactionHeader->transactionId,
               sizeof(transactionHeader->transactionId) );
    mem2int16( localMem,
               transactionHeader->transactionAuthorityId );
    mem2int32( localMem,
               transactionHeader->contentId );
    mem2bytes( localMem,
               transactionHeader->reserved,
               sizeof(transactionHeader->reserved) );

    *mem = localMem; /* point memory to last position */

    return DRM_ERROR_NONE;

}

int32_t mem2DrmMemory( uint8_t * * mem,
                       drmMemory_t * drmMemory,
                       aesKeyHandle keyHandle )
{
    uint8_t * localMem = *mem;
    /* uint8_t * startMem = *mem; */
    /* uint32_t length = 0; */
    /* uint32_t i = 0; */

    /* Serialize */ 
#if DRM_OTHER_SECURE_PLATFORM == 0
    mem2bytes( localMem,
               drmMemory->prefixPad,
               sizeof(drmMemory->prefixPad) );
    mem2bytes( localMem,
               drmMemory->randomPad1,
               sizeof(drmMemory->randomPad1) );
#endif
    mem2bytes( localMem,
               drmMemory->owner.guard,
               sizeof(drmMemory->owner.guard) );
#if DRM_OTHER_SECURE_PLATFORM == 0
    mem2int8( localMem,
              drmMemory->owner.randomPad1 );
#endif
    mem2bytes( localMem,
               drmMemory->owner.key,
               sizeof(drmMemory->owner.key) );
#if DRM_OTHER_SECURE_PLATFORM == 0
    mem2int8( localMem,
              drmMemory->owner.randomPad2 );
#endif
    mem2bytes( localMem,
               drmMemory->owner.userId,
               sizeof(drmMemory->owner.userId) );
#if DRM_OTHER_SECURE_PLATFORM == 0
    mem2int8( localMem,
              drmMemory->owner.randomPad3 );
#endif

#if (DRM_OTHER_SECURE_PLATFORM == 0)
    for ( i = 0; i < TOTAL_PLAY_SLOTS; i++ )
    {
        mem2bytes( localMem,
                   drmMemory->slots[i].serialNumber,
                   sizeof(drmMemory->slots[i].serialNumber) );
        mem2int8( localMem,
                  drmMemory->slots[i].counter );
        mem2int8( localMem,
                  drmMemory->slots[i].randomPad );
    }
#else
/*In the 48 byte configuration, we pack the slot counters into 6 bytes*/
    unpackSlots(localMem, drmMemory);
#endif

#if DRM_OTHER_SECURE_PLATFORM == 0
    mem2int32( localMem,
               drmMemory->activationFailureCnt );           
    mem2bytes( localMem,
               drmMemory->randomPad2,
               sizeof(drmMemory->randomPad2) );
    mem2bytes( localMem,
               drmMemory->postfixPad,
               sizeof(drmMemory->postfixPad) );
#endif
    *mem = localMem; /* point memory to last position */

    return DRM_ERROR_NONE;

}
