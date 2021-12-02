/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/DrmMessage.c#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

/*
 Notes: All the pack functions are very similar in logic.  If you find a defect
 in one, it is probably in all of them.
*/
#ifndef __KERNEL__

#include "DrmMessage.h"
#include "DrmKeyStore.h"
#include "DataEncodingBits.h"

#ifdef _MTK_BUILD_
#include "libc/memory.h"
#include "libc/string.h"
#else
#include <string.h>
#endif

#else

#include "DrmMessage.h"
#include "DrmKeyStore.h"
#include "DataEncodingBits.h"

#endif

#define PROTECTION_FORMAT_BITS          4
#define REGISTRATION_CODE_BITS			46

void wrapSecureMessage( drmMessagePacked_t *transformMessage)
{
    uint8_t current = 0;
    uint8_t carryNext = 0;
    uint8_t carryLast = 0;
    uint8_t maskHigh = 0x0F;
    uint8_t maskLow = 0xF0;
    uint8_t i = 0;
    uint8_t loopSize = 0;
	uint8_t protectionFormatFlag = 0;
	uint8_t messageKey[SIZEOF_MESSAGE_KEY_SIZE_BYTES] = {0};
	int32_t keyStoreStatus = 0;

	transformMessage->sizeInBits += PROTECTION_FORMAT_BITS;
    loopSize = encodingGetByteLoopSizeFromBits(transformMessage->sizeInBits);


	keyStoreStatus = keyStoreGetProtectionFormat( &protectionFormatFlag,
	                                              &transformMessage->message[0] );

	if (keyStoreStatus == -1) // Clear out message if key is bad. 
	{
		memset( transformMessage->message,
		        0,
		        MAX_MESSAGE_SIZE_BYTES );
        transformMessage->sizeInBits = 0;
		return;
	}
	
	if ( 0 != keyStoreGetMessageKey( protectionFormatFlag,
	                                 messageKey ) ) // Clear out message if key is bad.
	{
		memset(transformMessage->message, 0, MAX_MESSAGE_SIZE_BYTES);
        transformMessage->sizeInBits = 0;
        return;
	}

	carryLast = ( ( protectionFormatFlag << 4 ) & 0xFFFFFFF0 ); // Shift over to make room for message, the mask is to indicate the loss of 4-bits of precision on the value.

    // COPYPASTE_WARNING: This loop logic is very close to unwrapSecureMessage loop.
    for (i = 0; i < loopSize - 1; i++)
    {
        current = transformMessage->message[i];
        carryNext = current & maskHigh;
        current = current & maskLow;
        current = current >> 4;

		// Handle 4 bits of first byte.
		if (i == 0)
		{
			current = current ^ messageKey[i];
			current = current & maskHigh;
		}

        current = current | carryLast;

		// XOR in crypto.
		if (i != 0)
		{
			current = current ^ messageKey[i];
		}

        transformMessage->message[i] = current;
        carryLast = ( ( carryNext << 4 ) & 0xFFFFFFF0 ); // The mask is to indicate the loss of 4-bits of precision on the value.
    }
    transformMessage->message[i] = carryLast;
	memset(messageKey, 0xF1, SIZEOF_MESSAGE_KEY_SIZE_BYTES); // Clear out key.
}

void unwrapSecureMessage( drmMessagePacked_t *transformMessage )
{
    uint8_t current = 0;
    uint8_t carryNext = 0;
    uint8_t carryLast = 0;
    uint8_t maskHigh = 0x0F;
    uint8_t maskLow = 0xF0;
	int32_t i = 0;
    uint8_t loopSize = 0;
    uint8_t protectionFormatFlag = 0;
	uint8_t messageKey[SIZEOF_MESSAGE_KEY_SIZE_BYTES] = {0};


    loopSize = encodingGetByteLoopSizeFromBits(transformMessage->sizeInBits);
    protectionFormatFlag = transformMessage->message[0] >> 4;
	carryLast = 0;

	if ( 0 != keyStoreGetMessageKey( protectionFormatFlag,
	                                 messageKey ) ) // Clear out message if key is bad.
	{
		memset(transformMessage->message, 0, MAX_MESSAGE_SIZE_BYTES);
        transformMessage->sizeInBits = 0;
		return;
	}

    // COPYPASTE_WARNING: This loop logic is very close to wrapSecureMessage loop.
	for (i = (loopSize - 2); i >= 0; i--)
    {
        current = transformMessage->message[i];

  		// XOR in crypto.
		if (i != 0)
		{
			current = current ^ messageKey[i];
		}

        carryNext = current & maskLow;
        current = current & maskHigh;

		// Handle 4 bits of first byte.
		if (i == 0)
		{
			current = current ^ messageKey[i];
			current = current & maskHigh;
		}

        current = ( ( current << 4 ) & 0xFFFFFFF0 ); // the mask is to indicate the loss of 4-bits of precision on the value.
        current = current | carryLast;
        transformMessage->message[i] = current;
        carryLast = carryNext >> 4;
    }
    transformMessage->message[loopSize-2] |= (transformMessage->message[loopSize-1]>>4);
    transformMessage->message[loopSize-1] = 0;
    memset(messageKey, 0xF1, SIZEOF_MESSAGE_KEY_SIZE_BYTES); // Clear out key.
    transformMessage->sizeInBits -= PROTECTION_FORMAT_BITS;
}

void packdrmRegistrationRequest( drmRegistrationRequest_t request, drmMessagePacked_t *messagePacked)
{
    packRegistrationLogic(&request, messagePacked, (uint8_t) ENCODING_DIRECTION_B_BECOMES_A);
    registrationSecuritySwap(messagePacked);
}

void packRegistrationLogic(drmRegistrationRequest_t *request, drmMessagePacked_t *messagePacked, uint8_t direction)
{
    uint8_t packedIndex = 0;
    uint8_t guardIndex = 0;
    uint8_t modelIndex = 0;
    
    packedIndex = 0;
    
    for (guardIndex = 0; guardIndex < OWNER_GUARD_DMEM_BYTES; guardIndex++)
    {
        encodingAssign(&(request->userIdGuard[guardIndex]), &(messagePacked->message[packedIndex++]), direction);
    }
    for (modelIndex = 0; modelIndex < MODEL_ID_BYTES; modelIndex++)
    {
        encodingAssign(&(request->modelId[modelIndex]), &(messagePacked->message[packedIndex++]), direction);
    }

    if(direction == 1)
    {
        request->flag = (messagePacked->message[--packedIndex] & 0x0C) >> 2;
        request->modelId[MODEL_ID_BYTES-1] = request->modelId[MODEL_ID_BYTES-1] & 0xF0;
    }
    else
    {
        messagePacked->message[--packedIndex] |= (0x0C & (request->flag << 2));
    }
    
	messagePacked->sizeInBits = REGISTRATION_CODE_BITS;
}

void registrationSecuritySwap(drmMessagePacked_t *messagePacked)
{
	// Mix in random number with static model number.
	encodingHalfByteSwap(&(messagePacked->message[0]), &(messagePacked->message[4]), 0);
	encodingHalfByteSwap(&(messagePacked->message[2]), &(messagePacked->message[3]), 1);
}
