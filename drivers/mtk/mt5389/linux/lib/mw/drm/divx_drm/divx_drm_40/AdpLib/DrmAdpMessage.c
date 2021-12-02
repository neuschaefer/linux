/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_40/AdpLib/DrmAdpMessage.c#1 $
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

#include "DrmAdpMessage.h"

#include "AdpKeyStoreLib/DrmAdpKeyStore.h"

#include "CryptoLib/Bits.h"       /* assign() */

#ifdef _MTK_BUILD_
#include "libc/string.h"
#include "libc/memory.h"
#else
#include <string.h>
#endif

#else

#include "DrmAdpMessage.h"

#include "../AdpKeyStoreLib/DrmAdpKeyStore.h"

#include "../CryptoLib/Bits.h"       /* assign() */

#endif

#define PROTECTION_FORMAT_BITS          4
#define REGISTRATION_CODE_BITS			36

void wrapSecureMessage(DrmMessagePacked *transformMessage)
{
    uint8_t current;
    uint8_t carryNext;
    uint8_t carryLast;
    uint8_t maskHigh = 0x0F;
    uint8_t maskLow = 0xF0;
    uint8_t i;
    uint8_t loopSize;
	uint8_t protectionFormatFlag;
	uint8_t messageKey[ADP_MESSAGE_KEY_SIZE_BYTES] = {0};
	uint8_t keyStoreStatus = ERROR_KEYSTORE_NONE;

	transformMessage->sizeInBits += PROTECTION_FORMAT_BITS;
    loopSize = getByteLoopSizeFromBits(transformMessage->sizeInBits);
#ifdef SUPPORT_SIGNED_ACTIVATION
	keyStoreStatus = getProtectionFormat(&protectionFormatFlag);
#else
	/* Derive protection format id from message itself.  Do not use invalid key. */
	protectionFormatFlag = (transformMessage->message[0] % VALID_MESSAGE_KEY_TOTAL) + (MESSAGE_KEY_INDEX_TOTAL - VALID_MESSAGE_KEY_TOTAL);
#endif

	if (ERROR_KEYSTORE_NONE != keyStoreStatus) /* Clear out message if key is bad. */
	{
		memset(transformMessage->message, 0, MAX_MESSAGE_SIZE_BYTES);
		return;
	}
	keyStoreStatus = getMessageKey(protectionFormatFlag, messageKey);
	if (ERROR_KEYSTORE_NONE != keyStoreStatus) /* Clear out message if key is bad. */
	{
		memset(transformMessage->message, 0, MAX_MESSAGE_SIZE_BYTES);
		return;
	}

	carryLast = ( ( protectionFormatFlag << 4 ) & 0xFFFFFFF0 ); /* Shift over to make room for message, the mask is to indicate the loss of 4-bits of precision on the value. */

    /* COPYPASTE_WARNING: This loop logic is very close to unwrapSecureMessage loop. */
    for (i = 0; i < loopSize; i++)
    {
        current = transformMessage->message[i];
        carryNext = current & maskHigh;
        current = current & maskLow;
        current = current >> 4;

		/* Handle 4 bits of first byte. */
		if (i == 0)
		{
			current = current ^ messageKey[i];
			current = current & maskHigh;
		}

        current = current | carryLast;

		/* XOR in crypto. */
		if (i != 0)
		{
			current = current ^ messageKey[i];
		}

        transformMessage->message[i] = current;
        carryLast = ( ( carryNext << 4 ) & 0xFFFFFFF0 ); /* The mask is to indicate the loss of 4-bits of precision on the value. */
    }

	memset(messageKey, 0xF1, ADP_MESSAGE_KEY_SIZE_BYTES); /* Clear out key. */
}

void unwrapSecureMessage(DrmMessagePacked *transformMessage)
{
    uint8_t current;
    uint8_t carryNext;
    uint8_t carryLast;
    uint8_t maskHigh = 0x0F;
    uint8_t maskLow = 0xF0;
	int32_t i;
    uint8_t loopSize;
    uint8_t protectionFormatFlag;
	uint8_t messageKey[ADP_MESSAGE_KEY_SIZE_BYTES] = {0};
	uint8_t keyStoreStatus;

    loopSize = getByteLoopSizeFromBits(transformMessage->sizeInBits);
    protectionFormatFlag = transformMessage->message[0] >> 4;
	carryLast = 0;
	keyStoreStatus = getMessageKey(protectionFormatFlag, messageKey);
	if (ERROR_KEYSTORE_NONE != keyStoreStatus) /* Clear out message if key is bad. */
	{
		memset(transformMessage->message, 0, MAX_MESSAGE_SIZE_BYTES);
		return;
	}

    /* COPYPASTE_WARNING: This loop logic is very close to wrapSecureMessage loop. */
	for (i = (loopSize - 1); i >= 0; i--)
    {
        current = transformMessage->message[i];

  		/* XOR in crypto. */
		if (i != 0)
		{
			current = current ^ messageKey[i];
		}

        carryNext = current & maskLow;
        current = current & maskHigh;

		/* Handle 4 bits of first byte. */
		if (i == 0)
		{
			current = current ^ messageKey[i];
			current = current & maskHigh;
		}

        current = ( ( current << 4 ) & 0xFFFFFFF0 ); /* the mask is to indicate the loss of 4-bits of precision on the value. */
        current = current | carryLast;
        transformMessage->message[i] = current;
        carryLast = carryNext >> 4;
    }

    memset(messageKey, 0xF1, ADP_MESSAGE_KEY_SIZE_BYTES); /* Clear out key. */
    transformMessage->sizeInBits -= PROTECTION_FORMAT_BITS;
}

void packRegistrationRequest(RegistrationRequest request, DrmMessagePacked *messagePacked)
{
    packRegistrationLogic(&request, messagePacked, (uint8_t) B_BECOMES_A);
    registrationSecuritySwap(messagePacked);
}

void packRegistrationLogic(RegistrationRequest *request, DrmMessagePacked *messagePacked, uint8_t direction)
{
    uint8_t packedIndex;
    uint8_t guardIndex;
    uint8_t modelIndex;
    
    packedIndex = 0;
    
    for (guardIndex = 0; guardIndex < OWNER_GUARD_BYTES; guardIndex++)
    {
        assign(&(request->userIdGuard[guardIndex]), &(messagePacked->message[packedIndex++]), direction);
    }
    for (modelIndex = 0; modelIndex < MODEL_ID_BYTES; modelIndex++)
    {
        assign(&(request->modelId[modelIndex]), &(messagePacked->message[packedIndex++]), direction);
    }
    
	messagePacked->sizeInBits = REGISTRATION_CODE_BITS;
}

void registrationSecuritySwap(DrmMessagePacked *messagePacked)
{
	/* Mix in random number with static model number. */
	halfByteSwap(&(messagePacked->message[0]), &(messagePacked->message[4]), 0);
	halfByteSwap(&(messagePacked->message[2]), &(messagePacked->message[3]), 1);
}
