/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_40/AdpLib/DrmAdpApi.c#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

/* DRM = Digital Rights Management */
/* ADP = Authorized Disconnected Playback */

#ifndef __KERNEL__

#include "DrmAdpApi.h"
#include "DrmAdpTypes.h"
#include "DrmAdpMessage.h"

#include "AdpKeyStoreLib/DrmAdpKeyStore.h"

#include "AdpLocalLib/DrmAdpLocal.h"
#include "AdpLocalLib/DrmAdpKeyStoreLocal.h"

#include "CryptoLib/base64.h"
#include "CryptoLib/rijndael-api-fst.h"
#include "CryptoLib/Bits.h"
#include "CryptoLib/sha256.h"
#ifdef SUPPORT_SIGNED_ACTIVATION
#include "CryptoLib/rsadecrypt.h"
#endif

#ifdef _MTK_BUILD_
#include "libc/memory.h"
#include "libc/string.h"
#include "libc/stdlib.h"
#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif
#else
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#endif

#else

#include "DrmAdpApi.h"
#include "DrmAdpTypes.h"
#include "DrmAdpMessage.h"

#include "../AdpKeyStoreLib/DrmAdpKeyStore.h"

#include "../AdpLocalLib/DrmAdpLocal.h"
#include "../AdpLocalLib/DrmAdpKeyStoreLocal.h"

#include "../CryptoLib/base64.h"
#include "../CryptoLib/rijndael-api-fst.h"
#include "../CryptoLib/Bits.h"
#include "../CryptoLib/sha256.h"
#ifdef SUPPORT_SIGNED_ACTIVATION
#include "../CryptoLib/rsadecrypt.h"
#endif

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

#endif

/* Internal errors. */
#define ADP_BAD_PARAMETER                   10
#define DRM_ERROR_DECRYPTION_FAILED			11  /* from: drmDecryptFrame */
#define DRM_ERROR_UNRECOGNIZED_DRM_MODE		14  /* from: drmInitPlaybackContext */
#define DRM_ERROR_NOT_AUTH_USER             15  /* from: drmQueryRentalStatus, drmCommitPlayback */
#define DRM_ERROR_RENTAL_EXPIRED            16  /* from: drmQueryRentalStatus, drmCommitPlayback */
#define DRM_ERROR_NEED_MORE_RANDOM_SAMPLE	17  /* from: drmGetRegistrationCodeString, drmCommitPlayback */
#define DRM_ERROR_NOT_INITIALIZED			18  /* from: drmQueryRentalStatus */
#define DRM_ERROR_RESERVED               	19
#define DRM_ERROR_NOT_COMMITTED				20  /* from: drmDecryptFrame */
#define DRM_ERROR_NOT_RENTAL_QUERIED		21  /* from: drmQueryCgmsa */
#define DRM_ERROR_BAD_SLOT_NUMBER           22  /* from: drmQueryRentalStatus */
#define DRM_ERROR_NULL_GUARD_SET_SIGNAL		23	/* from: drmCommitPlayback (for certification testing support) */
#define DRM_ERROR_INVALID_ALIGNMENT			24	/* from: drmInitPlaybackContext */
#define DRM_ERROR_NOT_CGMSA_QUERIED         25  /* from: drmQueryAcptb */
#define DRM_ERROR_NOT_ACPTB_QUERIED         26  /* from: drmQueryDigitalProtection */
#define DRM_ERROR_NOT_DIGITAL_QUERIED       27  /* from: drmCommitPlayback */
#define DRM_ERROR_SIGNATURE_MISMATCH        28  /* from: drmInitPlaybackContext */
#define DRM_ERROR_NOT_KEY_QUERIED        29  /* from: drmQueryFrameKeys */
#define DRM_ERROR_NOT_AUDIO_QUERIED        30  /* from: drmQueryAudioProtection */

/* ADP Playback Context States. */
#define DRM_STATE_INITIALIZED				 1
#define DRM_STATE_RENTAL_QUERIED			 2 
#define DRM_STATE_CGMSA_QUERIED              3
#define DRM_STATE_ACPTB_QUERIED              4
#define DRM_STATE_DIGITAL_PROTECTION_QUERIED 5
#define DRM_STATE_COMMITTED					 6
#define DRM_STATE_FRAME_KEY_QUERIED			 7
#define DRM_STATE_AUDIO_PROTECTION_QUERIED	 8

#define RANDOM_BYTE_BUCKET_SIZE			6

static uint8_t randomByteBucket[RANDOM_BYTE_BUCKET_SIZE];
static uint32_t randomSamples = 0;
static uint8_t localTicker = 1;
static uint8_t adpLastError = ADP_SUCCESS;

static uint8_t to[sizeof(DrmHeader)];
static uint8_t from[sizeof(DrmHeader)];

static DrmUseLimitIdInfo useLimitIdInfo[USE_LIMITS] =
{
	{USE_LIMIT_ID_UNLIMITED, 0},
	{USE_LIMIT_ID_ONE, 1},
	{USE_LIMIT_ID_THREE, 3},
	{USE_LIMIT_ID_FIVE, 5},
	{USE_LIMIT_ID_SEVEN, 7},
	{USE_LIMIT_ID_TEN, 10},
	{USE_LIMIT_ID_TWENTY, 20},
	{USE_LIMIT_ID_THIRTY, 30}
};

static uint8_t nullGuard[OWNER_GUARD_BYTES] = {0,0,0};

/* Non-API helper functions. */
static uint8_t optionOutputProtection(PlaybackContext *context);
#ifdef SUPPORT_SIGNED_ACTIVATION
static uint8_t optionSignedActivation(PlaybackContext *context);
#endif
static uint8_t optionProtectedAudio(PlaybackContext *context);
static uint8_t optionVariableFrameKeyCount(PlaybackContext *context);
static uint8_t maskCheck(const uint8_t option, const uint8_t mask);
static uint8_t getSessionKey(PlaybackContext *context, uint8_t *sessionKey);  /* Increments use counter if appropriate. */
static uint8_t randomizeGuard(DrmMemory *memory);
#ifdef SUPPORT_SIGNED_ACTIVATION
static uint8_t processSignature(PlaybackContext *context);
#endif
static void drmAdpContextCpy(uint8_t *from, PlaybackContext *context);
static void mem2drmRentalRecord(DrmRentalRecord* rentalRecord, uint8_t* mem);
static void drmRentalRecord2mem(uint8_t* mem,DrmRentalRecord* rentalRecord);
static uint8_t getRapidUpdater(void);
static void waitSecond(void);
static void setLastError(const uint8_t code);
static void packDrmMemory(DrmMemory memory, DrmPackedMemory *packedMemory);
static void unpackDrmMemory(DrmPackedMemory packedMemory, DrmMemory *memory);
static void packLogic(DrmMemory *memory, DrmPackedMemory *packedMemory, uint8_t direction);
static uint8_t loadDrmMemory(DrmMemory *memory);

/* Non-API helper functions, other libraries are extern'ing into these functions. */
void mem2drmAdpTargetHeader(DrmAdpTargetHeader* drmAdpTH, uint8_t* mem);
void mem2drmActivateRecord(DrmActivateRecord* activateRecord, uint8_t* mem);
void drmAdpTargetHeader2mem(uint8_t* mem, DrmAdpTargetHeader* drmAdpTH);
void drmActivateRecord2mem(uint8_t* mem, DrmActivateRecord* activateRecord);
void mem2drmHeaderSignature(DrmHeaderSignature *signature, uint8_t *mem);
void drmHeaderSignature2mem(uint8_t *mem, DrmHeaderSignature *signature);

uint32_t getPlaybackContextSize(void)
{
	return sizeof(PlaybackContext);
}

uint32_t getStrdDataSize(void)
{
	return sizeof(DrmHeader) + sizeof(DrmHeaderSignature);
}

void setLastError(const uint8_t code)
{
	adpLastError = code;
}

uint8_t drmAdpGetLastError(void)
{
	return adpLastError;
}

/**
 * init value of s1, s2, s3
 */
static uint32_t ui4_s1 = 0xAB12CE7AUL;
static uint32_t ui4_s2 = 0x12DACED1UL;
static uint32_t ui4_s3 = 0xD131CD43UL;

static uint8_t drmRandomGet(uint32_t ui4_rng)
{
    if (ui4_rng == 0) 
    {
        return (uint8_t)ui4_rng;
    }

    ui4_s1 = (((ui4_s1 & 4294967294UL) << 12) ^ (((ui4_s1 << 13) ^ ui4_s1) >> 19));
    ui4_s2 = (((ui4_s2 & 4294967288UL) <<  4) ^ (((ui4_s2 <<  2) ^ ui4_s2) >> 25));
    ui4_s3 = (((ui4_s3 & 4294967280UL) << 17) ^ (((ui4_s3 <<  3) ^ ui4_s3) >> 11));

    return (uint8_t)((ui4_s1 ^ ui4_s2 ^ ui4_s3) % ui4_rng);
}

void drmSetRandomSample(void)
{
	randomSamples++;
	if (randomSamples < RANDOM_BYTE_BUCKET_SIZE)
	{
		randomByteBucket[randomSamples] = drmRandomGet((uint32_t)getRapidUpdater());
	}
	else
	{
		randomSamples--;
	}
}

uint32_t drmGetRandomSampleCounter(void)
{
	return randomSamples;
}

uint8_t getRapidUpdater(void)
{
	uint8_t result;

	result = (uint8_t) (localClock() & 0x000000FF);
	result += localTicker;
	localTicker++;

	return result;
}

void waitSecond(void)
{
	uint32_t tick = localClock();
	uint32_t expire = tick + localClockTicksPerSec();
		
	while (tick < expire)
	{
		tick = localClock();
	}
}

uint8_t drmGetRegistrationCodeString(char *registrationCodeString)
{
    uint8_t result = ADP_NOT_AUTHORIZED;
	uint8_t finalResult = ADP_LOCAL_SUCCESS;
	DrmMemory memory;
    DrmMessagePacked messagePacked;
    RegistrationRequest request;

    memset(&memory, 0, sizeof(DrmMemory));
    memset(&messagePacked, 0, sizeof(DrmMessagePacked));
    memset(&request, 0, sizeof(RegistrationRequest));    

    result = loadDrmMemory(&memory);
    if (ADP_LOCAL_SUCCESS == result)
    {
        /* Set data if null guard. */
		if (0 == memcmp(memory.owner.guard, nullGuard, OWNER_GUARD_BYTES))
		{
                    drmSetRandomSample();
                    drmSetRandomSample();
                    drmSetRandomSample();		
                    
			result = randomizeGuard(&memory);
			if (ADP_SUCCESS != result)
			{
				setLastError(result);
				return finalResult;
			}
			result = saveDrmMemory(&memory);
			if (ADP_LOCAL_SUCCESS != result)
			{
				setLastError(result);
				return finalResult;
			}
		}

		localSetModelId(request.modelId);
        memcpy(request.userIdGuard, memory.owner.guard, OWNER_GUARD_BYTES);
    
        /* Pack, wrap, and convert for human display. */
        packRegistrationRequest(request, &messagePacked);
        wrapSecureMessage(&messagePacked);
		base32Encode40Bits((unsigned char*)messagePacked.message, registrationCodeString);

		finalResult = ADP_SUCCESS;
    }
    
    return result;
}

uint8_t drmInitPlaybackContext(uint8_t *strdData, uint8_t *drmContext)
{
	int i = 0;
	keyInstance baseKeyInstance = {0};
	int decryptedBlockSize = 0;
	cipherInstance cipher;
	PlaybackContext *context = (PlaybackContext *) drmContext;
	uint8_t result;
	uint8_t finalResult = ADP_NOT_AUTHORIZED;
	char adpBaseHexKey[(ADP_BASE_KEY_SIZE_BYTES * HEX_OCTET_PER_BYTE) + 1] = {0};

	if(NULL == strdData || NULL == drmContext)
	{
		setLastError(ADP_BAD_PARAMETER);
		return finalResult;
	}

	/* Put into memory structure. */
	drmAdpContextCpy(strdData, context);

	/* Get base key. */
	result = getAdpBaseHexKeyFromId(context->baseKeyId, adpBaseHexKey);
	if (ERROR_KEYSTORE_NONE != result)
	{
		setLastError(result);
		return finalResult;
	}

	if (TRUE == cipherInit(&cipher, MODE_ECB, NULL))
	{
		if (TRUE == makeKey(&baseKeyInstance, DIR_DECRYPT, ADP_BASE_KEY_SIZE_BITS, adpBaseHexKey))
		{
			memset(to, 0, sizeof(DrmAdpTargetHeader));
			drmAdpTargetHeader2mem(from, &context->adpTarget);
	
			decryptedBlockSize = blockDecrypt(&cipher, &baseKeyInstance, from, sizeof(DrmAdpTargetHeader) * CHAR_BIT, to);
			if(decryptedBlockSize != (sizeof(DrmAdpTargetHeader) * CHAR_BIT))
			{
				setLastError(DRM_ERROR_DECRYPTION_FAILED);
				return finalResult;
			}

			/* Put in xor. */
			for (i = sizeof(DrmAdpTargetHeader) - 1; i > 0; i--)
			{
				to[i] = to[i] ^ to[i-1];
			}

			mem2drmAdpTargetHeader(&context->adpTarget, to);

			if (DRM_TYPE_PROTECTED_AUDIO == context->adpTarget.drmMode)
			{
				context->adpTarget.drmMode = context->adpTarget.drmSubMode;
			}

			if (!(DRM_TYPE_RENTAL == context->adpTarget.drmMode ||
				DRM_TYPE_PURCHASE == context->adpTarget.drmMode ||
				DRM_TYPE_BASE == context->adpTarget.drmMode ||
				DRM_TYPE_ACTIVATION_PURCHASE == context->adpTarget.drmMode ||
				DRM_TYPE_ACTIVATION_RENTAL == context->adpTarget.drmMode) )
			{
				setLastError(DRM_ERROR_UNRECOGNIZED_DRM_MODE);
				return finalResult;
			}

#ifdef SUPPORT_SIGNED_ACTIVATION
			/* Process signature: the signature, if it checks ok, will decrypt other parts of the header. */
			if (1 == optionSignedActivation(context))
			{
				result = processSignature(context);
				if (ADP_SUCCESS != result)
				{
					setLastError(result);
					return finalResult;
				}
			}
#endif

			if (0 == optionVariableFrameKeyCount(context))
			{
				context->adpTarget.frameKeyCount = VIDEO_KEY_COUNT_MAX;
			}

			finalResult = ADP_SUCCESS;
			context->state = DRM_STATE_INITIALIZED;
		}
		else
		{
			setLastError(ERROR_KEYSTORE_INVALID_BASE_KEY);
			return finalResult;
		}
	}
	else
	{
		setLastError(DRM_ERROR_DECRYPTION_FAILED);
		return finalResult;
	}

	return finalResult;
}

uint8_t drmQueryRentalStatus(uint8_t *drmContext, uint8_t *rentalMessageFlag, uint8_t *useLimit, uint8_t *useCount)
{
    uint8_t returnCode;
	DrmMemory memory;
	uint8_t i;
	PlaybackContext *context = (PlaybackContext *) drmContext;
	uint16_t memorySerialNumber;
	uint16_t fileSerialNumber;
	uint8_t finalResult = ADP_NOT_AUTHORIZED;

	*useLimit = 0;
	*useCount = 0;

	memset(&memory, 0, sizeof(DrmMemory));

	if (DRM_STATE_INITIALIZED != context->state)
	{
		setLastError(DRM_ERROR_NOT_INITIALIZED);
		return finalResult;
	}

	context->state = DRM_STATE_RENTAL_QUERIED;

	*rentalMessageFlag = FALSE;

	if (!(DRM_TYPE_RENTAL == context->adpTarget.drmMode ||
		  DRM_TYPE_ACTIVATION_RENTAL == context->adpTarget.drmMode))

	{
		finalResult = ADP_SUCCESS;
		return finalResult;
	}


	if (USE_LIMIT_ID_UNLIMITED == context->adpTarget.rentalRecord.useLimitId)
	{
		finalResult = ADP_SUCCESS;
		return finalResult;
	}

	*rentalMessageFlag = TRUE;

    returnCode = loadDrmMemory(&memory);
    
    if (ADP_SUCCESS == returnCode)
    {
		/* Check if correct user, only if not activation record. */
		if ((0 != memcmp(context->adpTarget.userId, memory.owner.userId, OWNER_USER_ID_BYTES)) &&
			(DRM_TYPE_RENTAL == context->adpTarget.drmMode ||
			 DRM_TYPE_PURCHASE == context->adpTarget.drmMode))
		{
			setLastError(DRM_ERROR_NOT_AUTH_USER);
			return finalResult;
		}

		/* Ensure slot number is in range. */
		if (context->adpTarget.rentalRecord.slotNumber >= TOTAL_PLAY_SLOTS)
		{
			setLastError(DRM_ERROR_BAD_SLOT_NUMBER);
			return finalResult;
		}

		memorySerialNumber = pack(memory.slots[context->adpTarget.rentalRecord.slotNumber].serialNumber[0],
								  memory.slots[context->adpTarget.rentalRecord.slotNumber].serialNumber[1]);
		fileSerialNumber = pack(context->adpTarget.rentalRecord.serialNumber[0],
								context->adpTarget.rentalRecord.serialNumber[1]);

		/* Check if correct serial number. Only check if it is not an activation. */
		if (!((DRM_TYPE_ACTIVATION_RENTAL == context->adpTarget.drmMode)  &&
			  0 == memcmp(context->adpTarget.activateRecord.memoryGuard, memory.owner.guard, OWNER_GUARD_BYTES)) )
		{
			if (fileSerialNumber < memorySerialNumber)
			{
				finalResult = ADP_RENTAL_EXPIRED;
				return finalResult;
			}
		}

		/* Find use limit id. */
		for (i = 0; i < USE_LIMITS; i++)
		{
			if (useLimitIdInfo[i].id == context->adpTarget.rentalRecord.useLimitId)
			{
				*useLimit = useLimitIdInfo[i].uses;
				break;
			}
		}

		if (fileSerialNumber == memorySerialNumber)
		{
			*useCount = memory.slots[context->adpTarget.rentalRecord.slotNumber].counter;
		}
		else
		{
			*useCount = 0;
		}

		/* If a first time, uses will be zero. */
		if (fileSerialNumber > memorySerialNumber)
		{
			*useCount = 0;
		}
		if (*useCount == *useLimit)
		{
			finalResult = ADP_RENTAL_EXPIRED;
		}
		else
		{
			finalResult = ADP_SUCCESS;
		}
    }

    return finalResult;
}

uint8_t drmQueryCgmsa(uint8_t *drmContext, uint8_t *cgmsaSignal)
{
	uint8_t result = ADP_NOT_AUTHORIZED;
	PlaybackContext *context = (PlaybackContext *) drmContext;

	if (DRM_STATE_RENTAL_QUERIED == context->state)
	{
		if (1 == optionOutputProtection(context))
		{
			*cgmsaSignal = context->adpTarget.outputProtectionFlags >> 
				(ACPTB_BITS + DIGITAL_PROTECTION_BITS + OUTPUT_SIGNAL_RESERVED_BITS);
		}
		else
		{
			*cgmsaSignal = 0;
		}
		context->state = DRM_STATE_CGMSA_QUERIED;
		result = ADP_SUCCESS;
	}
	else
	{
		setLastError(DRM_ERROR_NOT_RENTAL_QUERIED);
	}

	return result;
}

uint8_t drmQueryAcptb(uint8_t *drmContext, uint8_t *acptbSignal)
{
	uint8_t result = ADP_NOT_AUTHORIZED;
	PlaybackContext *context = (PlaybackContext *) drmContext;

	if (DRM_STATE_CGMSA_QUERIED == context->state)
	{
		if (1 == optionOutputProtection(context))
		{
			*acptbSignal = context->adpTarget.outputProtectionFlags >> 
				(DIGITAL_PROTECTION_BITS + OUTPUT_SIGNAL_RESERVED_BITS);
			*acptbSignal = ((0xFF << ACPTB_BITS) ^ (0xFF))
				& (*acptbSignal);  /* Zero out other bits that are not used. */
		}
		else
		{
			*acptbSignal = 0;
		}
		context->state = DRM_STATE_ACPTB_QUERIED;
		result = ADP_SUCCESS;
	}
	else
	{
		setLastError(DRM_ERROR_NOT_CGMSA_QUERIED);
	}

	return result;
}

uint8_t drmQueryDigitalProtection(uint8_t *drmContext, uint8_t *digitalProtectionSignal)
{
	uint8_t result = ADP_NOT_AUTHORIZED;
	PlaybackContext *context = (PlaybackContext *) drmContext;

	if (DRM_STATE_ACPTB_QUERIED == context->state)
	{
		if (1 == optionOutputProtection(context))
		{
			*digitalProtectionSignal = context->adpTarget.outputProtectionFlags >> 
				(OUTPUT_SIGNAL_RESERVED_BITS);
			*digitalProtectionSignal = ((0xFF << DIGITAL_PROTECTION_BITS) ^ (0xFF))
				& (*digitalProtectionSignal);  /* Zero out other bits that are not used. */
		}
		else
		{
			*digitalProtectionSignal = 0;
		}
		context->state = DRM_STATE_DIGITAL_PROTECTION_QUERIED;
		result = ADP_SUCCESS;
	}
	else
	{
		setLastError(DRM_ERROR_NOT_ACPTB_QUERIED);
	}

	return result;
}

uint8_t drmCommitPlayback(uint8_t *drmContext)
{
	uint8_t sessionKey[KEY_SIZE_BYTES] = {0};
	char sessionHexKey[(KEY_SIZE_BYTES * HEX_OCTET_PER_BYTE) + 1] = {0};
	uint8_t encryptedFrameKey[KEY_SIZE_BYTES];
	keyInstance sessionKeyInstance = {0};
	int i;
	int decryptedFrameKeySize = 0;
	cipherInstance cipher;
	PlaybackContext *context = (PlaybackContext *) drmContext;
	uint8_t result = ADP_SUCCESS;
	int32_t cryptoResult = TRUE;

	if (DRM_STATE_DIGITAL_PROTECTION_QUERIED != context->state)
	{
		setLastError(DRM_ERROR_NOT_DIGITAL_QUERIED);
		return ADP_NOT_AUTHORIZED;
	}

	/* Try to get the session key. */
	result = getSessionKey(context, sessionKey);
	if (ADP_SUCCESS != result)
	{
		return result; /* Trust setLastError from getSession was set. */
	}
	
	convertBinToHex(sessionKey, KEY_SIZE_BYTES, sessionHexKey);

	/* Populate the frame key table. */
	cryptoResult = cipherInit(&cipher, MODE_ECB, NULL);
	if(TRUE == cryptoResult)
	{
		cryptoResult = makeKey(&sessionKeyInstance, DIR_DECRYPT, VIDEO_KEY_SIZE_BITS, sessionHexKey);
		if (TRUE == cryptoResult)
		{
			for (i = 0; i < context->adpTarget.frameKeyCount; i++)
			{
				memcpy(encryptedFrameKey, context->frameKeys.keys[i], VIDEO_KEY_SIZE_BYTES);
			
				decryptedFrameKeySize = blockDecrypt(&cipher, &sessionKeyInstance, encryptedFrameKey, VIDEO_KEY_SIZE_BITS, context->frameKeys.keys[i]);
				if(decryptedFrameKeySize != VIDEO_KEY_SIZE_BITS)
				{
					setLastError(DRM_ERROR_DECRYPTION_FAILED);
					return ADP_NOT_AUTHORIZED;
				}		
			}
			context->state = DRM_STATE_COMMITTED;
			return ADP_SUCCESS;
		}
		else
		{
			setLastError(DRM_ERROR_DECRYPTION_FAILED);
			return ADP_NOT_AUTHORIZED;
		}
	}
	else
	{
		setLastError(DRM_ERROR_DECRYPTION_FAILED);
		return ADP_NOT_AUTHORIZED;
	}

    /* statement is unreachable */
	/* return ADP_NOT_AUTHORIZED; */

}

uint8_t drmQueryFrameKeys(uint8_t* drmContext, uint8_t* frame_keys_count, uint8_t* frame_keys)
{
	uint8_t result = ADP_NOT_AUTHORIZED;
	PlaybackContext *context = (PlaybackContext *) drmContext;

	if (DRM_STATE_COMMITTED == context->state)
	{
        *frame_keys_count = context->adpTarget.frameKeyCount;
        memcpy(frame_keys, context->frameKeys.keys, (uint8_t)(*frame_keys_count)*VIDEO_KEY_SIZE_BYTES);
                    
		context->state = DRM_STATE_FRAME_KEY_QUERIED;
		result = ADP_SUCCESS;
	}
	else
	{
		setLastError(DRM_ERROR_NOT_COMMITTED);
	}

	return result;
}

uint8_t drmQueryAudioProtection(uint8_t* drmContext, AudioCryptoInfo* audioCryptoInfo)
{
	uint8_t result = ADP_NOT_AUTHORIZED;
	PlaybackContext *context = (PlaybackContext *) drmContext;

	if (DRM_STATE_FRAME_KEY_QUERIED == context->state)
	{
		if (1 == optionProtectedAudio(context))
		{
	        audioCryptoInfo->flag = PROTECTED_AUDIO;
            memcpy(audioCryptoInfo->key, context->frameKeys.keys[AUDIO_USE_VIDEO_KEY_INDEX], AUDIO_KEY_SIZE_BYTES);
            audioCryptoInfo->offset = context->adpTarget.protectedAudioOffset;
            audioCryptoInfo->size   = context->adpTarget.protectedAudioCryptoSize;
		}
		else
		{
			audioCryptoInfo->flag = 0;
		}
		context->state = DRM_STATE_AUDIO_PROTECTION_QUERIED;
		result = ADP_SUCCESS;
	}
	else
	{
		setLastError(DRM_ERROR_NOT_KEY_QUERIED);
	}

	return result;
}

uint8_t drmIncreaseRental(PlaybackContext *context)
{
    uint8_t j;
    uint8_t returnCode = ADP_SUCCESS;
    uint8_t useLimit = 255;  /* Ensure use limit initialized to max for correct security logic. */
    DrmAdpTargetHeader fileInfo = context->adpTarget;    

    if (DRM_STATE_AUDIO_PROTECTION_QUERIED != context->state)
    {
        setLastError(DRM_ERROR_NOT_AUDIO_QUERIED);
        return ADP_NOT_AUTHORIZED;
    }

    if (DRM_TYPE_RENTAL == fileInfo.drmMode ||
        DRM_TYPE_ACTIVATION_RENTAL == fileInfo.drmMode)
    {   
        /* Find use limit id. */
        for (j = 0; j < USE_LIMITS; j++)
        {
            if (useLimitIdInfo[j].id == fileInfo.rentalRecord.useLimitId)
            {
                useLimit = useLimitIdInfo[j].uses;
                break;
            }
        }

        /* Check use limit id. */
        if ( (useLimit > context->t_drm_memory.slots[fileInfo.rentalRecord.slotNumber].counter) ||
             (USE_LIMIT_ID_UNLIMITED == fileInfo.rentalRecord.useLimitId) )
        {    
            /* Update counter, yes still does if unlimited.  This is ok. */
            context->t_drm_memory.slots[fileInfo.rentalRecord.slotNumber].counter++;
        }
        else
        {
            setLastError(DRM_ERROR_RENTAL_EXPIRED);
            return ADP_RENTAL_EXPIRED;
        }
    }

    return returnCode;
}

#if 0
uint8_t drmDecryptVideo(uint8_t *drmContext, uint8_t *frame, uint32_t frameSize, uint8_t *drmFrameInfo)
{
	PlaybackContext *context = (PlaybackContext *) drmContext;
	uint8_t result = ADP_NOT_AUTHORIZED;
	uint8_t returnCode;

	if (DRM_STATE_COMMITTED == context->state)
	{
		returnCode = decryptVideo(&context->frameKeys, frame, frameSize, drmFrameInfo);
		if (VIDEO_DECRYPT_ERROR_NONE == returnCode)
		{
			result = ADP_SUCCESS;
		}
		else
		{
			setLastError(DRM_ERROR_DECRYPTION_FAILED);
		}
	}
	else
	{
		setLastError(DRM_ERROR_NOT_COMMITTED);
	}

	return result;
}

uint8_t drmDecryptAudio(uint8_t *drmContext, uint8_t *frame, uint32_t frameSize)
{
	PlaybackContext *context = (PlaybackContext *) drmContext;
	uint8_t result = ADP_NOT_AUTHORIZED;
	AudioCryptoInfo audioSettings;
	uint8_t returnCode;
    uint8_t *pAudioSettingsKey, *pContextFrameKey;

	if (DRM_STATE_COMMITTED == context->state)
	{
		if (0 == optionProtectedAudio(context))
		{
			return ADP_SUCCESS; /* Ignore if no crypto. */
		}

     /* A bug in older versions of gcc such as the 2.5.95 version that causes memcpy to
       copy data offset by a byte which corrupts the audioSettings.key data 
       using individual pointers to the two parameters resolves the issue. */
        /* memcpy(audioSettings.key, context->frameKeys.keys[AUDIO_USE_VIDEO_KEY_INDEX], AUDIO_KEY_SIZE_BYTES); */
        pAudioSettingsKey = audioSettings.key;
        pContextFrameKey = context->frameKeys.keys[AUDIO_USE_VIDEO_KEY_INDEX];
        memcpy(pAudioSettingsKey, pContextFrameKey, AUDIO_KEY_SIZE_BYTES);
        
		audioSettings.offset = context->adpTarget.protectedAudioOffset;
		audioSettings.size   = context->adpTarget.protectedAudioCryptoSize;

		returnCode = decryptAudio(&audioSettings, frame, frameSize);

		if (AUDIO_DECRYPT_ERROR_NONE == returnCode)
		{
			result = ADP_SUCCESS;
		}
		else
		{
			setLastError(DRM_ERROR_DECRYPTION_FAILED);
		}

		if (ADP_SUCCESS != result)
		{
			setLastError(DRM_ERROR_DECRYPTION_FAILED);
		}
	}
	else
	{
		setLastError(DRM_ERROR_NOT_COMMITTED);
	}

	return result;
}
#endif

uint8_t randomizeGuard(DrmMemory *memory)
{
    uint8_t i;
	uint8_t randomByte;
	uint8_t returnCode = ADP_SUCCESS;

	memcpy(memory->owner.guard, nullGuard, OWNER_GUARD_BYTES);
	for (i = 0; i < OWNER_GUARD_BYTES; i++)
	{
		if (randomSamples > 0)
		{
			randomByte = randomByteBucket[randomSamples];
			randomSamples--;
		}
		else
		{
			setLastError(DRM_ERROR_NEED_MORE_RANDOM_SAMPLE);
			return ADP_NOT_AUTHORIZED;
		}

		memory->owner.guard[i] = randomByte;
	}

	/* Ensure guard cannot be null guard.  This should be a 2^24 chance. */
	if (0 == memcmp(memory->owner.guard, nullGuard, OWNER_GUARD_BYTES))
	{
		memory->owner.guard[0] = 1;
	}
	
	return returnCode;
}

/* This function is a bit long. Careful, it has multiple return statements. */
uint8_t getSessionKey(PlaybackContext *context, uint8_t *sessionKey)
{
    uint8_t returnCode = ADP_SUCCESS;
    uint8_t i;
	uint8_t j;
    uint8_t useLimit = 255;  /* Ensure use limit initialized to max for correct security logic. */
	DrmMemory memory;
	uint8_t clearKey[KEY_SIZE_BYTES] = {0};
	uint16_t fileSerialNumber = 0;
	uint16_t memorySerialNumber = 1;
	uint8_t userKeyMask[KEY_SIZE_BYTES] =
		{ 118, 151, 74, 140, 206, 221, 122, 171,
		  122, 86, 188, 103, 0, 52, 55, 76};
	DrmAdpTargetHeader fileInfo = context->adpTarget;

	/* Create a local copy of the device's modelId for determining if we are activating the correct model. */
	uint8_t modelId[MODEL_ID_BYTES];

        memset(&memory, 0, sizeof(DrmMemory));
        
	localSetModelId(modelId);

	if (DRM_TYPE_BASE == fileInfo.drmMode)
	{
		memcpy(sessionKey, fileInfo.sessionKey, KEY_SIZE_BYTES);
		return ADP_SUCCESS;
	}

	returnCode = loadDrmMemory(&memory);
    if (ADP_SUCCESS != returnCode)
    {
		setLastError(returnCode);
        return ADP_NOT_AUTHORIZED;
    }  

    /* Purchase logic. */
    if (DRM_TYPE_PURCHASE == fileInfo.drmMode /* ||
		DRM_TYPE_ACTIVATION_PURCHASE == fileInfo.drmMode*/)
    {        
		if (0 == memcmp(fileInfo.userId, memory.owner.userId, OWNER_USER_ID_BYTES))
		{
			for (i = 0; i < KEY_SIZE_BYTES; i++)
			{
				sessionKey[i] = fileInfo.sessionKey[i] ^ memory.owner.key[i];
			} 
		}
		else
		{
			setLastError(DRM_ERROR_NOT_AUTH_USER);
			return ADP_NOT_AUTHORIZED;
		}
		return ADP_SUCCESS;        
    }    

	/* Check if we need to activate.  Only activate signed files. */
	if ((DRM_TYPE_ACTIVATION_PURCHASE == fileInfo.drmMode ||
		 DRM_TYPE_ACTIVATION_RENTAL == fileInfo.drmMode) 
#ifdef SUPPORT_SIGNED_ACTIVATION
		 && (1 == optionSignedActivation(context))
#endif
		 )
	{
		/* Timer delay to prevent programmatic retry of activation. */
		waitSecond();;

		/* Check guard and model id. */
		if ((0 == memcmp(fileInfo.activateRecord.memoryGuard, memory.owner.guard, OWNER_GUARD_BYTES)) &&
			(0 == memcmp(fileInfo.activateRecord.modelId, modelId, MODEL_ID_BYTES)))
		{

			/* Check to see if we are being tested for initial guard of zero.  If so, return. */
			if (0 == memcmp(nullGuard, memory.owner.guard, OWNER_GUARD_BYTES))
			{
				setLastError(DRM_ERROR_NULL_GUARD_SET_SIGNAL);
				return ADP_NOT_AUTHORIZED;
			}

			/* Reset guard for next activation. */
			if (0 == memcmp(fileInfo.activateRecord.explicitGuard, nullGuard, OWNER_GUARD_BYTES))
			{
                            drmSetRandomSample();
                            drmSetRandomSample();
                            drmSetRandomSample();        

				/* Was set zero in file, so randomize it. */
				returnCode = randomizeGuard(&memory);
				if (returnCode == DRM_ERROR_NEED_MORE_RANDOM_SAMPLE)
				{
					setLastError(returnCode);
					return ADP_NOT_AUTHORIZED;
				}
			}
			else
			{
				/* Explicitly set the guard for next activation. */
				for (i = 0; i < OWNER_GUARD_BYTES; i++)
				{
					memory.owner.guard[i] = fileInfo.activateRecord.explicitGuard[i];
				}
			}
			
			/* Handle user key.  Xor record key before setting. */
			for (i = 0; i < KEY_SIZE_BYTES; i++)
			{
				clearKey[i] = fileInfo.activateRecord.userKey[i] ^ userKeyMask[i];
			}
			memcpy(memory.owner.key, clearKey, KEY_SIZE_BYTES);
			memset(clearKey, 0, KEY_SIZE_BYTES);

			/* User id. */
			memcpy(memory.owner.userId, fileInfo.userId, OWNER_USER_ID_BYTES);
			
			/* Reset slots. */
			for (i = 0; i < TOTAL_PLAY_SLOTS; i++)
			{
				memory.slots[i].counter = 0;
				memset(memory.slots[i].serialNumber, 0, SLOT_SERIAL_NUMBER_BYTES);
			}
#if 0
			returnCode = saveDrmMemory(&memory);
			if (ADP_SUCCESS != returnCode)
			{
				setLastError(returnCode);
				return ADP_NOT_AUTHORIZED;
			}
#else
                     context->b_save_drm_memory = DIVX_TRUE;
                     memcpy((void *)&(context->t_drm_memory), &memory, sizeof(DrmMemory));
#endif
		}
	}

    /* Purchase logic. */
    if (/*DRM_TYPE_PURCHASE == fileInfo.drmMode ||*/
		DRM_TYPE_ACTIVATION_PURCHASE == fileInfo.drmMode)
    {        
		if (0 == memcmp(fileInfo.userId, memory.owner.userId, OWNER_USER_ID_BYTES))
		{
			for (i = 0; i < KEY_SIZE_BYTES; i++)
			{
				sessionKey[i] = fileInfo.sessionKey[i] ^ memory.owner.key[i];
			} 
		}
		else
		{
			setLastError(DRM_ERROR_NOT_AUTH_USER);
			return ADP_NOT_AUTHORIZED;
		}
    }

	/* Rental logic. */
	if (DRM_TYPE_RENTAL == fileInfo.drmMode ||
		DRM_TYPE_ACTIVATION_RENTAL == fileInfo.drmMode)
	{
		/* Ensure correct user. */
		if (0 == memcmp(fileInfo.userId, memory.owner.userId, OWNER_USER_ID_BYTES))
		{
			/* Ensure slot number is in range. */
			if (fileInfo.rentalRecord.slotNumber >= TOTAL_PLAY_SLOTS)
			{
				setLastError(DRM_ERROR_BAD_SLOT_NUMBER);
				return ADP_NOT_AUTHORIZED;
			}

			/* Put serial number in form we can compare to. */
			memorySerialNumber = pack(memory.slots[fileInfo.rentalRecord.slotNumber].serialNumber[0],
			                    memory.slots[fileInfo.rentalRecord.slotNumber].serialNumber[1]);
            fileSerialNumber = pack(fileInfo.rentalRecord.serialNumber[0],
									fileInfo.rentalRecord.serialNumber[1]);

			/* Check first serial number. */
			if (fileSerialNumber > memorySerialNumber)
			{
				/* Update serial number. */
				memcpy(memory.slots[fileInfo.rentalRecord.slotNumber].serialNumber,
					   fileInfo.rentalRecord.serialNumber, SLOT_SERIAL_NUMBER_BYTES);
				memorySerialNumber = fileSerialNumber;

				/* Reset counter. */
				memory.slots[fileInfo.rentalRecord.slotNumber].counter = 0;
			}

			/* Check subsequent serial number. */
			if (fileSerialNumber == memorySerialNumber)
			{
				/* Find use limit id. */
				for (j = 0; j < USE_LIMITS; j++)
				{
					if (useLimitIdInfo[j].id == fileInfo.rentalRecord.useLimitId)
					{
						useLimit = useLimitIdInfo[j].uses;
						break;
					}
				}

				/* Check use limit id. */
				if ( (useLimit > memory.slots[fileInfo.rentalRecord.slotNumber].counter) ||
					 (USE_LIMIT_ID_UNLIMITED == fileInfo.rentalRecord.useLimitId) )
				{
				        #if 0
					/* Update counter, yes still does if unlimited.  This is ok. */
					memory.slots[fileInfo.rentalRecord.slotNumber].counter++;
                                    #endif

					/* And finally, copy over the key. */
					for (i = 0; i < KEY_SIZE_BYTES; i++)
					{
						sessionKey[i] = fileInfo.sessionKey[i] ^ memory.owner.key[i];
					} 
				}
				else
				{
					setLastError(DRM_ERROR_RENTAL_EXPIRED);
					return ADP_RENTAL_EXPIRED;
				}
			}
			else
			{
				setLastError(DRM_ERROR_RENTAL_EXPIRED);
				return ADP_RENTAL_EXPIRED;
			}
		}
		else
		{
			setLastError(DRM_ERROR_NOT_AUTH_USER);
			return ADP_NOT_AUTHORIZED;
		}
	}

    
    if (ADP_SUCCESS == returnCode)
    {
#if 0    
              returnCode = saveDrmMemory(&memory);
		if (ADP_SUCCESS != returnCode)
		{
			setLastError(returnCode);
			returnCode = ADP_NOT_AUTHORIZED;
		}
#else
              context->b_save_drm_memory = DIVX_TRUE;
              memcpy((void *)&(context->t_drm_memory), &memory, sizeof(DrmMemory));
#endif
    }
    
    return returnCode;
}

uint8_t optionOutputProtection(PlaybackContext *context)
{
	uint8_t result;
	
	result = maskCheck(context->adpTarget.optionFlags, OUTPUT_PROTECTION_MASK);
	
	return result;
}

#ifdef SUPPORT_SIGNED_ACTIVATION
uint8_t optionSignedActivation(PlaybackContext *context)
{
	uint8_t result; 

	result = maskCheck(context->adpTarget.optionFlags, SIGNED_ACTIVATION_MASK);

	return result;
}
#endif

uint8_t optionProtectedAudio(PlaybackContext *context)
{
	uint8_t result; 

	result = maskCheck(context->adpTarget.optionFlags, PROTECTED_AUDIO_MASK);

	return result;
}

uint8_t optionVariableFrameKeyCount(PlaybackContext *context)
{
	uint8_t result; 

	result = maskCheck(context->adpTarget.optionFlags, VARIABLE_FRAME_KEY_COUNT_MASK);

	return result;
}

uint8_t maskCheck(const uint8_t option, const uint8_t mask)
{
	uint8_t result = 0; /* 0 = false, 1 = true. */
	uint8_t check;

	check = option & mask;
	if (mask == check)
	{
		result = 1;
	}
	
	return result;
}

#ifdef SUPPORT_SIGNED_ACTIVATION
uint8_t processSignature(PlaybackContext *context)
{
	uint8_t result = ERROR_KEYSTORE_ID_NOT_FOUND;
	int32_t returnCode;
	uint8_t fileHash[SHA256_SIZE_BYTES] = {0};
	DrmDigest digest = {0};
	keyInstance digestKeyInstance = {0};
	char digestHexKey[(KEY_SIZE_BYTES * HEX_OCTET_PER_BYTE) + 1] = {0};
	int32_t amountDecrypted;
	cipherInstance cipher;
	uint8_t digestKeyHash[SHA256_SIZE_BYTES];
	int32_t i;
	uint8_t *activateRecordSubBlock;
	char nHex[ADP_RSA_HEX_KEY_SIZE_BYTES];
	char eHex[ADP_RSA_HEX_E_EXP_SIZE_BYTES];
	uint8_t fullSignedData[255];
	uint8_t adpTargetHeaderMem[sizeof(DrmAdpTargetHeader)];
	uint8_t activateRecordMem[sizeof(DrmActivateRecord)];


	drmAdpTargetHeader2mem(adpTargetHeaderMem, &(context->adpTarget));	
	sha256((unsigned char *) adpTargetHeaderMem, sizeof(DrmAdpTargetHeader), fileHash);

	result = getPublicKeyFromId(context->signature.signerId, nHex, eHex);

	if (ERROR_KEYSTORE_NONE == result)
	{
		result = DRM_ERROR_SIGNATURE_MISMATCH;
		returnCode = rsaPublicDecrypt255(nHex, eHex, context->signature.signedData, fullSignedData); /* Always returns 255 out. */
		memcpy(&digest, fullSignedData, sizeof(DrmDigest));  /* Only get digest portion, rest is random data. */

		if (returnCode == 0)
		{
			returnCode = memcmp(digest.hash, fileHash, SHA256_SIZE_BYTES);
			if (0 == returnCode)  /* Hashes match. */
			{
				/* Do the crypto with the digest key. */
				convertBinToHex(digest.key, KEY_SIZE_BYTES, digestHexKey);
				returnCode = cipherInit(&cipher, MODE_ECB, NULL);
				if(TRUE == returnCode)
				{
					returnCode = makeKey(&digestKeyInstance, DIR_DECRYPT, KEY_SIZE_BITS, digestHexKey);
					if (TRUE == returnCode)
					{
						drmActivateRecord2mem(activateRecordMem, &(context->adpTarget.activateRecord));

						amountDecrypted = blockDecrypt
							(&cipher,
							 &digestKeyInstance,
							 activateRecordMem,
							 KEY_SIZE_BITS * ADP_SIGNATURE_CRYPTO_BLOCKS,
							 activateRecordMem);

						mem2drmActivateRecord(&(context->adpTarget.activateRecord), activateRecordMem);

						if((KEY_SIZE_BITS * ADP_SIGNATURE_CRYPTO_BLOCKS) == amountDecrypted)
						{
							result = ADP_SUCCESS;
						}		
					}
					drmActivateRecord2mem(activateRecordMem, &(context->adpTarget.activateRecord));
					
					activateRecordSubBlock = activateRecordMem + KEY_SIZE_BYTES;
					sha256(digest.key, KEY_SIZE_BYTES, digestKeyHash);
					for (i = 0; i < ADP_SIGNATURE_SUB_CRYPTO_BYTES; i++)
					{
						activateRecordSubBlock[i] = activateRecordSubBlock[i] ^ digestKeyHash[i];
					}
					mem2drmActivateRecord(&(context->adpTarget.activateRecord), activateRecordMem);

				}
			}
		}
	}
	
	if (ADP_SUCCESS != result)
	{
		setLastError(result);
		result = ADP_NOT_AUTHORIZED;
	}

	return result;
}
#endif

void drmAdpContextCpy(uint8_t *from, PlaybackContext *context)
{
	uint8_t *drmHeaderPointer = from;
	int32_t signedFrameKeySize;
	
	drmHeaderPointer += sizeof(uint32_t);  /* Skip reserved flags. */
	memcpy(context->baseKeyId, drmHeaderPointer, sizeof(uint8_t)*ADP_BASE_KEY_ID_SIZE_BYTES);
	drmHeaderPointer += sizeof(uint8_t)*ADP_BASE_KEY_ID_SIZE_BYTES;

	mem2drmAdpTargetHeader(&context->adpTarget,drmHeaderPointer);
	drmHeaderPointer += sizeof(DrmAdpTargetHeader);
	
	drmHeaderPointer += sizeof(uint8_t)*TRANSACTION_ID_BYTES;
	drmHeaderPointer += sizeof(uint16_t);
	drmHeaderPointer += sizeof(uint32_t);
	drmHeaderPointer += sizeof(uint8_t)*DRM_OTHER_RESERVED;

	signedFrameKeySize = sizeof(uint8_t)*VIDEO_KEY_SIZE_BYTES*VIDEO_KEY_COUNT_FOR_SIGNED;

	
	memcpy(/*uint8_t*/context->frameKeys.keys, drmHeaderPointer, signedFrameKeySize);

	drmHeaderPointer += signedFrameKeySize;
	mem2drmHeaderSignature(&context->signature,drmHeaderPointer);

}

void mem2drmAdpTargetHeader(DrmAdpTargetHeader* drmAdpTH, uint8_t* mem)
{
	uint8_t *drmPointer = mem;
	
	drmAdpTH->drmMode = drmPointer[0] + (drmPointer[1] << 8);
	drmPointer += sizeof(uint16_t);
	memcpy(drmAdpTH->userId,drmPointer,sizeof(uint8_t)*OWNER_USER_ID_BYTES);
	drmPointer +=sizeof(uint8_t)*OWNER_USER_ID_BYTES;
	drmAdpTH->optionFlags = drmPointer[0];
	drmPointer += sizeof(uint8_t);

	mem2drmRentalRecord(&drmAdpTH->rentalRecord,drmPointer);
	drmPointer += sizeof(DrmRentalRecord);

	memcpy(drmAdpTH->sessionKey, drmPointer, sizeof(uint8_t)*KEY_SIZE_BYTES);
	drmPointer += sizeof(uint8_t)*KEY_SIZE_BYTES;

	mem2drmActivateRecord(&drmAdpTH->activateRecord,drmPointer);
	drmPointer += sizeof(DrmActivateRecord);

	drmAdpTH->outputProtectionFlags = drmPointer[0];
	drmPointer += sizeof(uint8_t);
	drmAdpTH->protectedAudioOffset = drmPointer[0];
	drmPointer += sizeof(uint8_t);
	drmAdpTH->protectedAudioCryptoSize = drmPointer[0];
	drmPointer += sizeof(uint8_t);

	drmAdpTH->frameKeyCount = drmPointer[0];
	drmPointer += sizeof(uint8_t);

	drmAdpTH->drmSubMode = drmPointer[0] + (drmPointer[1] << 8);
	drmPointer += sizeof(uint16_t);

	memcpy(drmAdpTH->reserved, drmPointer, sizeof(uint8_t)*DRM_ADP_RESERVED);
	drmPointer += sizeof(uint8_t)*DRM_ADP_RESERVED;
}

void mem2drmRentalRecord( DrmRentalRecord* rentalRecord, uint8_t* mem)
{
	uint8_t *drmPointer = mem;
	
	/*uint16_t*/rentalRecord->useLimitId = drmPointer[0] + (drmPointer[1] << 8);
	drmPointer += sizeof(uint16_t);
	memcpy(/*uint8_t*/rentalRecord->serialNumber, drmPointer, sizeof(uint8_t)*SLOT_SERIAL_NUMBER_BYTES);
	drmPointer += sizeof(uint8_t)*SLOT_SERIAL_NUMBER_BYTES;
	/*uint8_t*/rentalRecord->slotNumber = drmPointer[0];
	drmPointer += sizeof(uint8_t);
	memcpy(/*uint8_t*/rentalRecord->reserved, drmPointer, sizeof(uint8_t)*3);
	drmPointer += sizeof(uint8_t)*3;
}

void mem2drmActivateRecord( DrmActivateRecord* activateRecord, uint8_t* mem)
{
	uint8_t *drmPointer = mem;
	
	memcpy(activateRecord->memoryGuard, drmPointer, sizeof(uint8_t)*OWNER_GUARD_BYTES);
	drmPointer += sizeof(uint8_t)*OWNER_GUARD_BYTES;
	memcpy(activateRecord->modelId, drmPointer, sizeof(uint8_t)*MODEL_ID_BYTES);
	drmPointer += sizeof(uint8_t)*MODEL_ID_BYTES;
	memcpy(activateRecord->userKey, drmPointer, sizeof(uint8_t)*KEY_SIZE_BYTES);
	drmPointer += sizeof(uint8_t)*KEY_SIZE_BYTES;
	memcpy(activateRecord->explicitGuard, drmPointer, sizeof(uint8_t)*OWNER_GUARD_BYTES);
	drmPointer += sizeof(uint8_t)*OWNER_GUARD_BYTES;
}

void drmAdpTargetHeader2mem( uint8_t* mem, DrmAdpTargetHeader* drmAdpTH)
{
	uint8_t *drmPointer = mem;
	
	drmPointer[0] = drmAdpTH->drmMode & 0xFF;
	drmPointer[1] = (drmAdpTH->drmMode  >> 8) & 0xFF;
	drmPointer += sizeof(uint16_t);

	memcpy(drmPointer,drmAdpTH->userId,sizeof(uint8_t)*OWNER_USER_ID_BYTES);
	drmPointer +=sizeof(uint8_t)*OWNER_USER_ID_BYTES;
	drmPointer[0]=drmAdpTH->optionFlags;
	drmPointer += sizeof(uint8_t);

	drmRentalRecord2mem(drmPointer,&drmAdpTH->rentalRecord);
	drmPointer += sizeof(DrmRentalRecord);

	memcpy(drmPointer, drmAdpTH->sessionKey,sizeof(uint8_t)*KEY_SIZE_BYTES);
	drmPointer += sizeof(uint8_t)*KEY_SIZE_BYTES;

	drmActivateRecord2mem(drmPointer,&drmAdpTH->activateRecord);
	drmPointer += sizeof(DrmActivateRecord);

	drmPointer[0]=drmAdpTH->outputProtectionFlags;
	drmPointer += sizeof(uint8_t);

	drmPointer[0]=drmAdpTH->protectedAudioOffset;
	drmPointer += sizeof(uint8_t);

	drmPointer[0]=drmAdpTH->protectedAudioCryptoSize;
	drmPointer += sizeof(uint8_t);

	drmPointer[0]=drmAdpTH->frameKeyCount;
	drmPointer += sizeof(uint8_t);

	drmPointer[0] = drmAdpTH->drmSubMode & 0xFF;
	drmPointer[1] = (drmAdpTH->drmSubMode  >> 8) & 0xFF;
	drmPointer += sizeof(uint16_t);

	memcpy( drmPointer,drmAdpTH->reserved, sizeof(uint8_t)*DRM_ADP_RESERVED);
	drmPointer += sizeof(uint8_t)*DRM_ADP_RESERVED;
}
void drmRentalRecord2mem(uint8_t* mem,DrmRentalRecord* rentalRecord)
{
	uint8_t *drmPointer = mem;
	
	drmPointer[0] = rentalRecord->useLimitId & 0xFF; 
	drmPointer[1] = (rentalRecord->useLimitId >> 8) & 0xFF; 
	drmPointer += sizeof(uint16_t);
	memcpy(drmPointer, rentalRecord->serialNumber, sizeof(uint8_t)*SLOT_SERIAL_NUMBER_BYTES);
	drmPointer += sizeof(uint8_t)*SLOT_SERIAL_NUMBER_BYTES;
	drmPointer[0] = rentalRecord->slotNumber;
	drmPointer += sizeof(uint8_t);
	memcpy(drmPointer, rentalRecord->reserved,sizeof(uint8_t)*DRM_ADP_RENTAL_RESERVED_BYTES);
	drmPointer += sizeof(uint8_t)*DRM_ADP_RENTAL_RESERVED_BYTES;
}

void drmActivateRecord2mem(uint8_t* mem, DrmActivateRecord* activateRecord)
{
	uint8_t *drmPointer = mem;
	
	memcpy(drmPointer, activateRecord->memoryGuard,sizeof(uint8_t)*OWNER_GUARD_BYTES);
	drmPointer += sizeof(uint8_t)*OWNER_GUARD_BYTES;
	memcpy(drmPointer, activateRecord->modelId,sizeof(uint8_t)*MODEL_ID_BYTES);
	drmPointer += sizeof(uint8_t)*MODEL_ID_BYTES;
	memcpy(drmPointer, activateRecord->userKey,sizeof(uint8_t)*KEY_SIZE_BYTES);
	drmPointer += sizeof(uint8_t)*KEY_SIZE_BYTES;
	memcpy(drmPointer,activateRecord->explicitGuard,sizeof(uint8_t)*OWNER_GUARD_BYTES);
	drmPointer += sizeof(uint8_t)*OWNER_GUARD_BYTES;
}

void drmHdr2mem(uint8_t* mem, DrmHeader* drmHeader)
{
	uint8_t *drmHeaderPointer = mem;
	
	if((drmHeader == NULL) || (mem == NULL))
	{
		return;
	}

	drmHeaderPointer[0] = drmHeader->reservedFlags & 0xFF ;
	drmHeaderPointer[1] = (drmHeader->reservedFlags >> 8) & 0xFF;
	drmHeaderPointer[2] = (drmHeader->reservedFlags >> 16) & 0xFF;
	drmHeaderPointer[3] = (drmHeader->reservedFlags >> 24) & 0xFF;
	drmHeaderPointer += sizeof(uint32_t);
	memcpy(drmHeaderPointer,drmHeader->baseKeyId, sizeof(uint8_t)*DRM_BASE_KEY_ID_LENGTH);
	drmHeaderPointer += sizeof(uint8_t)*DRM_BASE_KEY_ID_LENGTH;

	drmAdpTargetHeader2mem( drmHeaderPointer, &drmHeader->adpTarget);
	drmHeaderPointer += sizeof(DrmAdpTargetHeader);


	memcpy(drmHeaderPointer,drmHeader->transaction.transactionId, sizeof(uint8_t)*TRANSACTION_ID_BYTES); 
	drmHeaderPointer += sizeof(uint8_t)*TRANSACTION_ID_BYTES;

	drmHeaderPointer[0] = drmHeader->transaction.transactionAuthorityId & 0xFF ;
	drmHeaderPointer[1] = (drmHeader->transaction.transactionAuthorityId >> 8) & 0xFF;
	drmHeaderPointer += sizeof(uint16_t);

	drmHeaderPointer[0] = drmHeader->transaction.contentId & 0xFF ;
	drmHeaderPointer[1] = (drmHeader->transaction.contentId >> 8) & 0xFF;
	drmHeaderPointer[2] = (drmHeader->transaction.contentId >> 16) & 0xFF;
	drmHeaderPointer[3] = (drmHeader->transaction.contentId >> 24) & 0xFF;
	drmHeaderPointer += sizeof(uint32_t);

	memcpy(drmHeaderPointer,drmHeader->transaction.reserved, sizeof(uint8_t)*DRM_OTHER_RESERVED);
	drmHeaderPointer += sizeof(uint8_t)*DRM_OTHER_RESERVED;
	memcpy(drmHeaderPointer, drmHeader->frameKeys, sizeof(uint8_t)*16*128);
	

}


void packDrmMemory(DrmMemory memory, DrmPackedMemory *packedMemory)
{
    packLogic(&memory, packedMemory, (uint8_t) B_BECOMES_A);
}

void unpackDrmMemory(DrmPackedMemory packedMemory, DrmMemory *memory)
{
    packLogic(memory, &packedMemory, (uint8_t) A_BECOMES_B);
}

void packLogic(DrmMemory *memory, DrmPackedMemory *packedMemory, uint8_t direction)
{
    uint8_t packedIndex;  /* Keep incrementing as we assign to unpacked memory. */
    uint8_t slotIndex;
    uint8_t multiUseIndex;  /* Uses to count byte placement in DrmMemory fields. */
    
    packedIndex = 0;
    
    /* Get owner. */
    for (multiUseIndex = 0; multiUseIndex < OWNER_GUARD_BYTES; multiUseIndex++)
    {
        assign(&(memory->owner.guard[multiUseIndex]), &(packedMemory->packed[packedIndex++]), direction);
    }
    for (multiUseIndex = 0; multiUseIndex < KEY_SIZE_BYTES; multiUseIndex++)
    {
        assign(&(memory->owner.key[multiUseIndex]), &(packedMemory->packed[packedIndex++]), direction);
    }
    for (multiUseIndex = 0; multiUseIndex < OWNER_USER_ID_BYTES; multiUseIndex++)
    {
        assign(&(memory->owner.userId[multiUseIndex]), &(packedMemory->packed[packedIndex++]), direction);
    }
    
	/* Get slots. */
    for (slotIndex = 0; slotIndex < TOTAL_PLAY_SLOTS; slotIndex++)
    {
        for (multiUseIndex = 0; multiUseIndex < SLOT_SERIAL_NUMBER_BYTES; multiUseIndex++)
        {
            assign(&(memory->slots[slotIndex].serialNumber[multiUseIndex]), &(packedMemory->packed[packedIndex++]), direction);
        }
          
        assign(&(memory->slots[slotIndex].counter), &(packedMemory->packed[packedIndex++]), direction);
    }
}

uint8_t loadDrmMemory(DrmMemory *memory)
{
	DrmPackedMemory memoryPacked;
    uint8_t returnCode = ADP_SUCCESS;

    memset(&memoryPacked, 0, sizeof(DrmPackedMemory));
    

	returnCode = localLoadDrmMemory(memoryPacked.packed);

    if (ADP_LOCAL_SUCCESS == returnCode)
    {
        unpackDrmMemory(memoryPacked, memory);
    }
	else
	{
		setLastError(returnCode);
		returnCode = ADP_NOT_AUTHORIZED;
	}
    
    return returnCode;
}

uint8_t saveDrmMemory(DrmMemory *memory)
{
	DrmPackedMemory memoryPacked;
    uint8_t returnCode = ADP_SUCCESS;

    memset(&memoryPacked, 0, sizeof(DrmPackedMemory));
    
    packDrmMemory(*memory, &memoryPacked);
    
	returnCode = localSaveDrmMemory(memoryPacked.packed);
    
    if (ADP_LOCAL_SUCCESS == returnCode)
    {
        unpackDrmMemory(memoryPacked, memory);
    }
	else
	{
		setLastError(returnCode);
		returnCode = ADP_NOT_AUTHORIZED;
	}
    
    return returnCode;
}

void drmHeaderSignature2mem(uint8_t *mem, DrmHeaderSignature *signature)
{
	uint8_t *drmPointer = mem;
	
	drmPointer[0] = signature->signerId & 0xFF; 
	drmPointer[1] = (signature->signerId >> 8) & 0xFF; 
	drmPointer[1] = (signature->signerId >> 16) & 0xFF; 
	drmPointer[1] = (signature->signerId >> 24) & 0xFF; 
	drmPointer += sizeof(uint32_t);
	memcpy(drmPointer, signature->reserved, sizeof(uint8_t)*ADP_SIGNATURE_RESERVED_BYTES);
	drmPointer += sizeof(uint8_t)*ADP_SIGNATURE_RESERVED_BYTES;
	memcpy(drmPointer, signature->signedData, sizeof(uint8_t)*ADP_RSA_KEY_SIZE_BYTES);


}
void mem2drmHeaderSignature(DrmHeaderSignature *signature, uint8_t *mem)
{
	uint8_t *drmPointer = mem;
	
	signature->signerId = drmPointer[0] + (drmPointer[1] << 8) + (drmPointer[1] << 16) + (drmPointer[1] << 24);
	
	drmPointer += sizeof(uint32_t);
	memcpy(signature->reserved, drmPointer,sizeof(uint8_t)*ADP_SIGNATURE_RESERVED_BYTES);
	drmPointer += sizeof(uint8_t)*ADP_SIGNATURE_RESERVED_BYTES;
	memcpy(signature->signedData, drmPointer,sizeof(uint8_t)*ADP_RSA_KEY_SIZE_BYTES);


}

