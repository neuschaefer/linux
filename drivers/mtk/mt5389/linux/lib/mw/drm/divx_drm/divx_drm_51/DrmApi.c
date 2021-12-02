/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/DrmApi.c#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/
#ifndef __KERNEL__

#include "DrmApi.h"
#include "DrmTypes.h"
#include "DrmMessage.h"
#include "DrmDataLoad.h"
#include "HeaderMem2Structs.h"
#include "HeaderStructs2Mem.h"

#include "DrmLocal.h"

#include "DivXPortableAPI.h"
#include "DivXAesDecrypt.h"
#include "DivXSha.h"
#include "DivXRng.h"
#include "DivXRsa.h"
#include "DivXRsaEncrypt.h"
#include "DivXRsaDecrypt.h"

#include "DataEncodingBase64.h"
#include "DataEncodingBits.h"

#if defined DRM_MOBILE_SYMBIAN_PLATFORM
const
#endif
DIVX_PORTABLE_MODULES_BEGIN
    DIVX_PORTABLE_ADD_MODULE(Crypto)
DIVX_PORTABLE_MODULES_END

#ifdef _MTK_BUILD_
#include "libc/memory.h"
#include "libc/string.h"
#include "libc/stdlib.h"
#include "libc/stdio.h"
#else
#if 0
#include "DivXVersion.h"
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#endif

#else

#include "DrmApi.h"
#include "DrmTypes.h"
#include "DrmMessage.h"
#include "DrmDataLoad.h"
#include "HeaderMem2Structs.h"
#include "HeaderStructs2Mem.h"

#include "local/DrmLocal.h"

#include "crypto/DivXPortableAPI.h"
#include "crypto/DivXAesDecrypt.h"
#include "crypto/DivXSha.h"
#include "crypto/DivXRng.h"
#include "crypto/DivXRsa.h"
#include "crypto/DivXRsaEncrypt.h"
#include "crypto/DivXRsaDecrypt.h"

#include "DataEncodingBase64.h"
#include "DataEncodingBits.h"

#if defined DRM_MOBILE_SYMBIAN_PLATFORM
const
#endif
DIVX_PORTABLE_MODULES_BEGIN
    DIVX_PORTABLE_ADD_MODULE(Crypto)
DIVX_PORTABLE_MODULES_END

#endif

// Internal errors.
#define DRM_ERROR_BAD_PARAMETER             10
#define DRM_ERROR_DECRYPTION_FAILED			11  // from: drmDecryptFrame
#define DRM_ERROR_UNRECOGNIZED_DRM_MODE		14  // from: drmInitplaybackContext_t
#define DRM_ERROR_NOT_AUTH_USER             15  // from: drmQueryRentalStatus, drmCommitPlayback
#define DRM_ERROR_RENTAL_EXPIRED            16  // from: drmQueryRentalStatus, drmCommitPlayback
#define DRM_ERROR_NEED_MORE_RANDOM_SAMPLE	17  // from: drmGetRegistrationCodeString, drmCommitPlayback
#define DRM_ERROR_NOT_INITIALIZED			18  // from: drmQueryRentalStatus
#define DRM_ERROR_RESERVED               	19
#define DRM_ERROR_NOT_COMMITTED				20  // from: drmDecryptFrame
#define DRM_ERROR_NOT_RENTAL_QUERIED		21  // from: drmQueryCgmsa
#define DRM_ERROR_BAD_SLOT_NUMBER           22  // from: drmQueryRentalStatus
#define DRM_ERROR_NULL_GUARD_SET_SIGNAL		23	// from: drmCommitPlayback (for certification testing support)
#define DRM_ERROR_INVALID_ALIGNMENT			24	// from: drmInitplaybackContext_t
#define DRM_ERROR_NOT_CGMSA_QUERIED         25  // from: drmQueryAcptb
#define DRM_ERROR_NOT_ACPTB_QUERIED         26  // from: drmQueryDigitalProtection
#define DRM_ERROR_NOT_DIGITAL_QUERIED       27  // from: drmCommitPlayback
#define DRM_ERROR_SIGNATURE_MISMATCH        28  // from: drmInitplaybackContext_t
#if 1
#define DRM_ERROR_NOT_KEY_QUERIED        29  /* from: drmQueryFrameKeys */
#define DRM_ERROR_NOT_AUDIO_QUERIED        30  /* from: drmQueryAudioProtection */
#endif

#if 0 /* Move to DrmApi.h */
// Playback Context States.
typedef enum drmApiState
{
    DRM_STATE_BOOT = 1,
    DRM_STATE_INITIALIZED,
    DRM_STATE_RENTAL_QUERIED,
    DRM_STATE_CGMSA_QUERIED,
    DRM_STATE_ACPTB_QUERIED,
    DRM_STATE_DIGITAL_PROTECTION_QUERIED,
    DRM_STATE_COMMITTED,
    DRM_STATE_FINALIZED
} drmApiState_t;

#define RANDOM_BYTE_BUCKET_SIZE			8
#endif

#if 0 /* Move to DrmApi.h */
#define AUDIO_KEY_SIZE_BYTES				16
#define AUDIO_CRYPTO_BLOCK_SIZE_BYTES		16
#define AUDIO_KEY_SIZE_BITS					128
#define AUDIO_MAX_ENCRYPT_BYTES             128
#define AUDIO_USE_VIDEO_KEY_INDEX           0
#endif

#define KEY_INDEX_SIZE 2
#define OFFSET_SIZE    4
#define CRYPTO_SIZE    4

/*------------------------------------------------------------------------
//  STRUCTURES
//-----------------------------------------------------------------------*/
#if 0 /* Move to DrmApi.h */
typedef struct audioCryptoInfo
{
    aesKeyHandle key;
	uint16_t offset;
	uint16_t size;
} audioCryptoInfo_t;
#endif

static const uint8_t nullGuard[OWNER_GUARD_DMEM_BYTES] = {0,0,0,0};
static const uint8_t nullUserId[OWNER_USER_ID_BYTES] = { 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t initUserId[OWNER_USER_ID_BYTES] = { 0xAC, 0xAC, 0xAC, 0xAC, 0xAC};
static const uint8_t deactUserId[OWNER_USER_ID_BYTES] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

static const drmUseLimitIdInfo_t useLimitIdInfo[USE_LIMITS] =
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


#if 0 /* Move to DrmApi.h */
typedef struct playbackContext
{
	uint8_t baseKeyId[SIZEOF_BASE_KEY_ID_SIZE_BYTES];
	uint8_t* strdPtr;
    drmTargetHeader_t targetHeader;
    drmTransactionInfoHeader_t transactionHeader;
    drmFrameKeys_t frameKeys;
	aesKeyHandle keys[VIDEO_KEY_COUNT_MAX];
    uint32_t randomSamples;
    uint8_t randomByteBucket[RANDOM_BYTE_BUCKET_SIZE];
    uint8_t localTicker;
	drmApiState_t state;
	drmErrorCodes_t drmLastError;
    drmHeaderSignature_t  signature;
} playbackContext_t;
#endif

static uint8_t s_maskCheck( const uint8_t option,
                            const uint8_t mask );
static drmErrorCodes_t s_getSessionKey( playbackContext_t *context,
                                        uint8_t *sessionKey );  // Increments use counter if appropriate. 
static drmErrorCodes_t s_randomizeGuard( playbackContext_t *context,
                                         drmMemory_t *memory);
/*
static void s_setLastError( uint8_t* drmContext,
                            const uint8_t code );
*/                            
int32_t s_activationGenerateProtectedMessage( uint8_t* messageBuffer,
                                              rsaPublicKeyHandle rsaPublicKey,
                                              int8_t* code,
                                              uint8_t* guard,
                                              uint8_t flags );
static void s_activationMsgStruct2Mem( uint8_t* toBuffer,
                                       drmActivationMessage_t* fromMessage );
static void s_activationProtectedMsgStruct2Mem( uint8_t* toBuffer,
                                                drmActivationProtectedMessage_t* fromMessage );
static uint8_t s_processSignature(playbackContext_t *context);
/*------------------------------------------------------------------------
//  LOCAL MEMORY HELPERS
//-----------------------------------------------------------------------*/
static drmErrorCodes_t s_loadDrmMemory( drmMemory_t *memory );
#if 0
static drmErrorCodes_t s_saveDrmMemory( drmMemory_t *memory );
#endif

/*------------------------------------------------------------------------
//  VIDEO ENCRYPTION/DECRYPTION
//-----------------------------------------------------------------------*/
static int32_t s_decryptAudio( audioCryptoInfo_t *audioSettings,
                               uint8_t *frame,
                               uint32_t frameSize );
                        
/*
 * 
 */
uint32_t drmGetDrmContextSize( void )
{
	return sizeof(playbackContext_t);
}

/*
 * 
 */
uint32_t drmGetStrdSize( void )
{
	return sizeof(drmHeader_t);
}

/*
 * 
 */
void setLastError(  uint8_t* drmContext,
                    const drmErrorCodes_t code )
{
    playbackContext_t *context = (playbackContext_t *) drmContext;
	context->drmLastError = code;
}

/*
 * 
 */
drmErrorCodes_t drmGetLastError( uint8_t* drmContext )
{
    playbackContext_t *context = (playbackContext_t *) drmContext;
	return context->drmLastError;
}

#if 1
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
#endif

/*
 * 
 */
drmErrorCodes_t drmSetRandomSample( uint8_t* drmContext )
{
	playbackContext_t *context = (playbackContext_t *) drmContext;

    if ( drmContext == NULL )
    {
		setLastError( drmContext,
		              (drmErrorCodes_t)DRM_ERROR_BAD_PARAMETER );
		return DRM_GENERAL_ERROR;
    }
	
	context->randomSamples++;
	
	if ( context->randomSamples < RANDOM_BYTE_BUCKET_SIZE )
	{
	    uint8_t result;

        result = (uint8_t) (localClock() & 0x000000FF);
        result += context->localTicker;
        context->localTicker++;

		context->randomByteBucket[context->randomSamples] = drmRandomGet((uint32_t)result);
	}
	else
	{
		context->randomSamples--;
	}
	return DRM_SUCCESS;
}

/*
 * 
 */
uint32_t drmGetRandomSampleCounter( uint8_t* drmContext )
{
	playbackContext_t *context = (playbackContext_t *) drmContext;

    if ( drmContext == NULL )
    {
		setLastError( drmContext,
		              (drmErrorCodes_t)DRM_ERROR_BAD_PARAMETER );
		return DRM_GENERAL_ERROR;
    }
	
	return context->randomSamples;
}

/*
 * 
 */
drmErrorCodes_t drmGetRegistrationCodeString( uint8_t* drmContext,
                                              char *registrationCodeString )
{

#if DRM_BASE_ENCRYPTION_ONLY == 1
    return DRM_GENERAL_ERROR;
#else
	playbackContext_t *context = (playbackContext_t *) drmContext;
    drmErrorCodes_t result = DRM_NOT_AUTHORIZED;
    uint32_t finalResult = DRM_LOCAL_SUCCESS;
    drmMemory_t memory = {0};
    drmMessagePacked_t messagePacked = {0};
    drmRegistrationRequest_t request = {0};
    /* int8_t protectionFlag = -1; */

    if (   ( drmContext == NULL )
        || ( registrationCodeString == NULL ) )
    {
		return DRM_GENERAL_ERROR;
    }

#if 1
    drmSetRandomSample(drmContext);
    drmSetRandomSample(drmContext);
    drmSetRandomSample(drmContext);
    drmSetRandomSample(drmContext);    
#endif
    
    if ( drmGetRandomSampleCounter( drmContext ) < SET_RANDOM_SAMPLE_SEED_MIN )
    {
		setLastError( drmContext,
		              result );
        return result;
    }

    result = s_loadDrmMemory( &memory );
    
    /* If the DRM memory failed to create, just create new one.  No harm no foul.
       This just means the user must re-register if they delete the memory or it get
       altered somehow.
       
       The server will pick up abnormal activation behaviour so we're not too concerned
       if the memory is consistently being deleted. */
    if ( DRM_LOCAL_SUCCESS != result )
    {
        memset( &memory,
                0,
                sizeof(memory) );
#if DRM_OTHER_SECURE_PLATFORM == 0
        memset( memory.prefixPad,
                0xA5,
                4 );
        memset( memory.postfixPad,
                0xA5,
                4 );
#endif
        result = s_saveDrmMemory( &memory );
        result = s_loadDrmMemory( &memory );
    }
    
    if ( DRM_LOCAL_SUCCESS == result )
    {
        // Set data if null guard.
		if ( 0 == memcmp( memory.owner.guard,
                          nullGuard,
                          OWNER_GUARD_DMEM_BYTES ) )
		{
			result = s_randomizeGuard( context,
			                           &memory );
			if ( DRM_SUCCESS != result )
			{
				setLastError( drmContext,
		                      result );
				return (drmErrorCodes_t)finalResult;
			}
			result = s_saveDrmMemory( &memory );
			if ( DRM_LOCAL_SUCCESS != result )
			{
				setLastError( drmContext,
		                      result );
				return (drmErrorCodes_t)finalResult;
			}
		}

		drmDataLoadModelId( request.modelId );
        memcpy( request.userIdGuard,
                memory.owner.guard,
                OWNER_GUARD_DMEM_BYTES );
    
        request.flag = drmDataLoadProfile();
        // Pack, wrap, and convert for human display.
        packdrmRegistrationRequest( request,
                                      &messagePacked );
        wrapSecureMessage( &messagePacked );
        if(messagePacked.sizeInBits == 0)
        {
            return DRM_NOT_AUTHORIZED;
        }
		encodingBase32Encode50Bits( (unsigned char*)messagePacked.message,
                                    (int8_t *)registrationCodeString );

		finalResult = DRM_SUCCESS;
    }
    return result;

#endif
}

int8_t drmIsDeviceActivated( void )
{

    uint8_t userId[OWNER_USER_ID_BYTES] = {0};
    uint32_t userIdLength = OWNER_USER_ID_BYTES;
    int8_t ret = DRM_SUCCESS;

    ret = drmGetActivationStatus(userId, &userIdLength);
    if(ret == DRM_SUCCESS)
    {

        if( (memcmp(userId, nullUserId, userIdLength) == 0)||
            (memcmp(userId, initUserId, userIdLength) == 0)||
            (memcmp(userId, deactUserId, userIdLength) == 0))
        {
            ret = DRM_DEVICE_NOT_ACTIVATED;
        }
        else
        {
            ret = DRM_DEVICE_IS_ACTIVATED;
        }
    }
    else if(ret == DRM_NOT_REGISTERED)
    {
        ret = DRM_DEVICE_NOT_ACTIVATED;
    }
    return ret;

}

/*
 * 
 */
drmErrorCodes_t drmGetActivationStatus( uint8_t* userId,
                                        uint32_t* userIdLength )
{
    drmMemory_t memory = {0};
    drmErrorCodes_t result = DRM_NOT_AUTHORIZED;

    if (   ( userId == NULL )
        || ( userIdLength == NULL ) )
    {
        return DRM_GENERAL_ERROR;
    }

    if ( *userIdLength < OWNER_USER_ID_BYTES )
    {
        return DRM_GENERAL_ERROR;
    }
    
    result = s_loadDrmMemory( &memory );

    /* If the DRM memory failed to create the there's no user yet. */
    if ( DRM_LOCAL_SUCCESS != result )
    {
        userId[0] = 0;
        *userIdLength = 0;
        return DRM_NOT_REGISTERED;
    }
    else
    {
        if ( memory.owner.userId[0] == 0 )
        {
            userId[0] = 0;
            *userIdLength = 0;
            return DRM_NOT_REGISTERED;
        }
        else
        {
            memcpy( userId,
                    memory.owner.userId,
                    OWNER_USER_ID_BYTES );
            *userIdLength = OWNER_USER_ID_BYTES;
        }
    }
    
    memset( &memory,
            0,
            sizeof(drmMemory_t) );

    return DRM_SUCCESS;
}

/*
 * 
 */
drmErrorCodes_t drmGetDeactivationCodeString( uint8_t* drmContext,
                                              char *deactivationCodeString )
{
#if DRM_BASE_ENCRYPTION_ONLY == 1
    return DRM_GENERAL_ERROR;
#else
    drmMemory_t drmMemoryStruct;
    uint8_t drmMemoryHashBuffer[80];
    uint8_t drmMemoryBuffer[80];
    uint8_t clearedDrmMemory[80];
    uint8_t *buffer = drmMemoryBuffer;
    uint8_t deactivationDataHash[32];
    uint32_t deactivationDataHashSize = 32;
    /* char base32Encoded[9] = { 0 }; */
    
    /* int result = 0; */
    /* int i = 0; */
    /* int j = 0; */

    /* STEP 0: Clear out the DRM memory */
    memset( drmMemoryHashBuffer,
            0,
            sizeof(drmMemoryHashBuffer) );
    memset( drmMemoryBuffer,
            0,
            sizeof(drmMemoryBuffer) );
           
    /* STEP 1: Load the real memory into a structure. */
    if ( 0 != drmDataLoadLoadDrmMemory( drmMemoryBuffer,
                                        PACKED_ALLOCATION_BYTES ) )
    {
        return DRM_GENERAL_ERROR;
    }
    
    if ( 0 != mem2DrmMemory( &buffer,
                             &drmMemoryStruct,
                             0 ) )
    {
        return DRM_GENERAL_ERROR;
    }

    /* STEP 2: Create the memory we're going to hash. */
    memset( drmMemoryHashBuffer,
            0xA5, /*prefixPad*/
            4 );
    memset( drmMemoryHashBuffer + 76,/*offset to postfixPad in 80 byte DrmMemory model*/
            0xA5,
            4 );

    /*Initialize the memory we'll commit to DRM Memory*/
    memcpy(clearedDrmMemory, drmMemoryHashBuffer, sizeof(drmMemoryHashBuffer));

    /* Copy over the guard value */
    memcpy( drmMemoryHashBuffer + 8,
            drmMemoryStruct.owner.guard,
            sizeof(drmMemoryStruct.owner.guard) );

    /* Set the user id value to 0xFFFFF */
    memset( drmMemoryHashBuffer + OFFSET_TO_USERID_DRMMEM_80BYTE,
            0xFF,
            sizeof(drmMemoryStruct.owner.userId) );

    /* STEP 3: Set up the memory we're going to commit to DRM memory. */
#if DRM_OTHER_SECURE_PLATFORM == 0    
    /* Copy over the guard value */
    memcpy( clearedDrmMemory + 8,
            drmMemoryStruct.owner.guard,
            sizeof(drmMemoryStruct.owner.guard) );
    /* Set the user id value to 0xFFFFF */
    memset( clearedDrmMemory + OFFSET_TO_USERID_DRMMEM_80BYTE,
            0xFF,
            sizeof(drmMemoryStruct.owner.userId) );
#else
    /* Copy over the guard value */
    memcpy( clearedDrmMemory,
            drmMemoryStruct.owner.guard,
            sizeof(drmMemoryStruct.owner.guard) );

    /* Set the user id value to 0xFFFFF */
    memset( clearedDrmMemory + OFFSET_TO_USERID_DRMMEM_48BYTE,
            0xFF,
            sizeof(drmMemoryStruct.owner.userId) );
#endif 

    /* STEP 4: Commit to memory, this operation is permament
               and clear the structure containing the old DRM memory */
    memset( &drmMemoryStruct,
            0xCC,
            sizeof(drmMemoryStruct) );

    if ( 0 != drmDataLoadSaveDrmMemory( clearedDrmMemory,
                                        PACKED_ALLOCATION_BYTES ) )
    {
        return DRM_GENERAL_ERROR;
    }

    /* STEP 5: Create the deregistration string. */
    /*result = */DivXSha256( (uint8_t*)drmMemoryHashBuffer,
                         80, /*size of the 80 byte drmMemory*/
                         deactivationDataHash,
                         &deactivationDataHashSize );

    encodingBase32Encode40Bits( deactivationDataHash,
                                (int8_t *)deactivationCodeString );

    return DRM_SUCCESS;
#endif

}

/*
 * 
 */
drmErrorCodes_t drmGetActivationMessage( uint8_t* drmContext,
                                         int8_t* activationMessage,
                                         uint32_t* activationMessageLength )
{
#if DRM_BASE_ENCRYPTION_ONLY == 1
    return DRM_GENERAL_ERROR;
#else
    const int8_t *begin = "--- BEGIN DIVXAM ---";
    const int8_t *end = "--- END DIVXAM ---";
    const int8_t *null = "\0";
	 /* playbackContext_t *context = (playbackContext_t *) drmContext; */
    rsaPublicKeyHandle rsaPublicKey;
    drmMemory_t drmMemoryStruct;

    uint8_t drmMemoryBuffer[sizeof(drmMemory_t)];
    uint8_t *buffer = drmMemoryBuffer;
    uint8_t protectedMessageBuffer[sizeof(drmActivationProtectedMessage_t)];
    int8_t regcode[11];
    uint32_t retVal = 0;
    uint32_t base64Length = 0;
    uint8_t publicKeyE[2];
    uint8_t publicKeyN[256];

    if ( activationMessage == NULL )
    {
        *activationMessageLength = (uint32_t)strlen( (const char *)begin ) + SIZEOF_ACTIVATION_MSG_BASE64 + (uint32_t)strlen( (const char *)end );
        return DRM_SUCCESS;
    }
    else
    {
        if ( *activationMessageLength < ( (uint32_t)strlen( (const char *)begin ) + SIZEOF_ACTIVATION_MSG_BASE64 + (uint32_t)strlen( (const char *)end ) ) )
        {
            return DRM_GENERAL_ERROR;
        }
    }

    if ( 0 != drmDataLoadLoadDrmMemory( drmMemoryBuffer,
                                        PACKED_ALLOCATION_BYTES ) )
    {
        return DRM_GENERAL_ERROR;
    }
    
    if ( 0 != mem2DrmMemory( &buffer,
                             &drmMemoryStruct,
                             0 ) )
    {
        return DRM_GENERAL_ERROR;
    }
                       
    retVal = drmGetRegistrationCodeString( drmContext,
                                           (char *)regcode );
    if ( retVal != DRM_SUCCESS )
    {
        return DRM_GENERAL_ERROR;
    }
    if ( 0 != drmDataLoadGetPublicKey( publicKeyE,
                                       publicKeyN ) )
    {
        return DRM_GENERAL_ERROR;
    }
    DivXRsaCreatePublicKey( &rsaPublicKey,
                            publicKeyN,
                            (int32_t)sizeof(publicKeyN),
                            publicKeyE,
                            (int32_t)sizeof(publicKeyE) );

    retVal = s_activationGenerateProtectedMessage( protectedMessageBuffer,
                                                   rsaPublicKey,
                                                   regcode,
                                                   drmMemoryStruct.owner.guard,
                                                   DRM_ACTIVATION_MESSAGE_FLAG );
    DivXRsaDeletePublicKey( rsaPublicKey );
    if ( retVal != 0 )
    {
        return DRM_GENERAL_ERROR;
    }
    
    strncpy( (char *)activationMessage,
             (const char *)begin,
             strlen( (const char *)begin ) );
    if ( encodingBase64Encode( (int8_t*)protectedMessageBuffer, 
                               sizeof(drmActivationProtectedMessage_t),
                               activationMessage + strlen( (const char *)begin ),
                               &base64Length ) == -1 )
    {
        return DRM_GENERAL_ERROR;
    }

    strncpy( (char *)activationMessage + strlen( (const char *)begin ) + base64Length,
             (const char *)end,
             strlen( (const char *)end ) );
    strncpy( (char *)activationMessage + strlen( (const char *)begin ) + base64Length + strlen( (const char *)end ),
             (const char *)null,
             strlen( (const char *)null ) );
    return DRM_SUCCESS;
#endif

}

/*
        This API will create an deactivation message
 */
drmErrorCodes_t drmGetDeactivationMessage( uint8_t* drmContext,
                                           int8_t* messageString,
                                           uint32_t* messageStringLength  )
{

#if DRM_BASE_ENCRYPTION_ONLY == 1
    return DRM_GENERAL_ERROR;
#else
    const int8_t* begin = "--- BEGIN DIVXDM ---";
    const int8_t* end = "--- END DIVXDM ---";
    const int8_t* null = "\0";
	 /* playbackContext_t *context = (playbackContext_t *) drmContext; */
    rsaPublicKeyHandle rsaPublicKey;
    drmMemory_t drmMemoryStruct;

    uint8_t drmMemoryBuffer[sizeof(drmMemory_t)];
    uint8_t *buffer = drmMemoryBuffer;
    uint8_t protectedMessageBuffer[sizeof(drmActivationProtectedMessage_t)];
    int8_t regcode[9];
    uint32_t retVal = 0;
    uint32_t base64Length = 0;
    uint8_t publicKeyE[2];
    uint8_t publicKeyN[256];

    if ( messageString == NULL )
    {
        *messageStringLength = (uint32_t)strlen( (const char *)begin ) + SIZEOF_ACTIVATION_MSG_BASE64 + (uint32_t)strlen( (const char *)end );
        return DRM_SUCCESS;
    }
    else
    {
        if ( *messageStringLength < (uint32_t)strlen( (const char *)begin ) + SIZEOF_ACTIVATION_MSG_BASE64 + (uint32_t)strlen( (const char *)end ) )
        {
            return DRM_GENERAL_ERROR;
        }
    }

    retVal = drmGetDeactivationCodeString( drmContext,
                                           (char *)regcode );
    if ( retVal != DRM_SUCCESS )
    {
        return DRM_GENERAL_ERROR;
    }	

    if ( 0 != drmDataLoadLoadDrmMemory( drmMemoryBuffer,
                                        PACKED_ALLOCATION_BYTES ) )
    {
        return DRM_GENERAL_ERROR;
    }
    
    if ( 0 != mem2DrmMemory( &buffer,
                             &drmMemoryStruct,
                             0 ) )
    {
        return DRM_GENERAL_ERROR;
    }

    if ( 0 != drmDataLoadGetPublicKey( publicKeyE,
                                       publicKeyN ) )
    {
        return DRM_GENERAL_ERROR;
    }
    DivXRsaCreatePublicKey( &rsaPublicKey,
                            publicKeyN,
                            (int32_t)sizeof(publicKeyN),
                            publicKeyE,
                            (int32_t)sizeof(publicKeyE) );

    retVal = s_activationGenerateProtectedMessage( protectedMessageBuffer,
                                                   rsaPublicKey,
                                                   regcode,
                                                   drmMemoryStruct.owner.guard,
                                                   DRM_DEACTIVATION_MESSAGE_FLAG );
    DivXRsaDeletePublicKey( rsaPublicKey );
    if ( retVal != 0 )
    {
        return DRM_GENERAL_ERROR;
    }
    
    strncpy( (char *)messageString,
             (const char *)begin,
             strlen( (const char *)begin ) );
    if ( encodingBase64Encode( (int8_t*)protectedMessageBuffer, 
                               sizeof(drmActivationProtectedMessage_t),
                               messageString + strlen( (const char *)begin ),
                               &base64Length ) == -1 )
    {
        return DRM_GENERAL_ERROR;
    }

    strncpy( (char *)messageString + strlen( (const char *)begin ) + base64Length,
             (const char *)end,
             strlen( (const char *)end ) );
    strncpy( (char *)messageString + strlen( (const char *)begin ) + base64Length + strlen((const char *) end ),
             (const char *)null,
             strlen( (const char *)null ) );

    return DRM_SUCCESS;
#endif

}


/*
 * 
 */
drmErrorCodes_t drmInitSystem( uint8_t *drmContext,
                              uint32_t *drmContextLength )
{
	playbackContext_t *context = (playbackContext_t *) drmContext;
	uint32_t i = 0;
	
	if (   ( drmContext == NULL )
	    && ( drmContextLength != NULL ) )
    {
        *drmContextLength = drmGetDrmContextSize( );
        return DRM_SUCCESS;
    }
    else
    {
	    if (   (   ( drmContext == NULL )
	            && ( drmContextLength == NULL ) )
	        || ( *drmContextLength != drmGetDrmContextSize( ) ) )
	           
        {
            return DRM_GENERAL_ERROR;
        }
    }

    /* Initialize a few of the context variables */
    context->drmLastError = DRM_SUCCESS;
	context->randomSamples = 0;
	context->localTicker = 1;
    for ( i = 0; i < sizeof(context->keys)/sizeof(aesKeyHandle); i++ )
    {
        context->keys[i] = 0;
    }
    context->state = (drmApiState_t)0;
    context->state = DRM_STATE_BOOT;
    return DRM_SUCCESS;

}
                              

/*
 * 
 */
drmErrorCodes_t drmInitPlayback( uint8_t *drmContext,
                                 uint8_t *strdData )
{
	/* int32_t i = 0; */
	playbackContext_t *context = (playbackContext_t *) drmContext;
	aesKeyHandle keyHandle;

	drmErrorCodes_t finalResult = DRM_NOT_AUTHORIZED;
	uint8_t baseKey[SIZEOF_BASE_KEY_SIZE_BYTES] = {0};
	drmErrorCodes_t result = DRM_NOT_AUTHORIZED;
    uint8_t *signaturePtr = NULL;
    /* uint8_t *activationRecPtr = NULL; */

    /* Initalize portable interfaces */
    DivXPortableInitialize ( );

    if ( context->state != DRM_STATE_BOOT )
    {
        return DRM_NOT_AUTHORIZED;
    }

	if (   ( NULL == strdData )
        || ( NULL == drmContext ) )
	{
		setLastError( drmContext,
		              (drmErrorCodes_t)DRM_ERROR_BAD_PARAMETER );
		return finalResult;
	}

	/* Put into playback context */
	{
	    uint8_t *drmHeaderPointer = strdData;

        /* Skip reserved flags. */
	    drmHeaderPointer += sizeof(uint32_t);
	
	    /* Copy out the base key ID. */
	    memcpy( context->baseKeyId,
                drmHeaderPointer,
                SIZEOF_BASE_KEY_ID_SIZE_BYTES );
        drmHeaderPointer += SIZEOF_BASE_KEY_ID_SIZE_BYTES;
        context->strdPtr = drmHeaderPointer;
    }

	/* Get base key. */
	if ( 0 != drmDataLoadGetBaseKey( context->baseKeyId,
                                     baseKey ) )
	{
	    result = DRM_NOT_AUTHORIZED;
		setLastError( drmContext,
		              result );
		return result;
	}

	if ( 0 == DivXAesCreateKey( &keyHandle,
                                aesKeySize32Bytes,
                                baseKey ) )
	{
	    mem2TargetHeader( &context->strdPtr,
                          &context->targetHeader,
                          keyHandle );
        DivXAesDeleteKey( keyHandle );
    }

    if ( DRM_TYPE_PROTECTED_AUDIO == context->targetHeader.drmMode )
	{
		context->targetHeader.drmMode = context->targetHeader.drmSubMode;
	}

#if DRM_BASE_ENCRYPTION_ONLY == 1
	if (  (   DRM_TYPE_RENTAL == context->targetHeader.drmMode
           || DRM_TYPE_PURCHASE == context->targetHeader.drmMode
           || DRM_TYPE_ACTIVATION_PURCHASE == context->targetHeader.drmMode
           || DRM_TYPE_ACTIVATION_RENTAL == context->targetHeader.drmMode ) )
	{
		setLastError( drmContext,
		              DRM_ERROR_UNRECOGNIZED_DRM_MODE );
		return finalResult;
	}
#else
	if ( !(   DRM_TYPE_RENTAL == context->targetHeader.drmMode
           || DRM_TYPE_PURCHASE == context->targetHeader.drmMode
           || DRM_TYPE_BASE == context->targetHeader.drmMode
           || DRM_TYPE_ACTIVATION_PURCHASE == context->targetHeader.drmMode
           || DRM_TYPE_ACTIVATION_RENTAL == context->targetHeader.drmMode ) )
	{
		setLastError( drmContext,
		              (drmErrorCodes_t)DRM_ERROR_UNRECOGNIZED_DRM_MODE );
		return finalResult;
	}
#endif

    if((DRM_TYPE_ACTIVATION_PURCHASE == context->targetHeader.drmMode) ||
       (DRM_TYPE_ACTIVATION_RENTAL == context->targetHeader.drmMode))
    {

    	if (1 == s_maskCheck(context->targetHeader.optionFlags, 
                        SIGNED_ACTIVATION_MASK) )
	    {
            signaturePtr = strdData + sizeof(drmHeader_t);  
            /*NOTE: write a function to copy this structure properly*/
            memcpy(&(context->signature), signaturePtr, sizeof(drmHeaderSignature_t));
            if (s_processSignature(context) != DRM_SUCCESS)
		    {
			    setLastError(drmContext, result);
			    return finalResult;
		    }
	    }
    }

	if ( 0 == s_maskCheck( context->targetHeader.optionFlags,
                           VARIABLE_FRAME_KEY_COUNT_MASK ) )
	{
		context->targetHeader.frameKeyCount = VIDEO_KEY_COUNT_MAX;
	}

	finalResult = DRM_SUCCESS;
	context->state = DRM_STATE_INITIALIZED;

	return finalResult;
}

/*
 * 
 */
drmErrorCodes_t drmQueryRentalStatus( uint8_t *drmContext,
                                      uint8_t *rentalMessageFlag,
                                      uint8_t *useLimit,
                                      uint8_t *useCount )
{
    uint8_t guard[OWNER_GUARD_DMEM_BYTES];
    drmErrorCodes_t returnCode = DRM_NOT_AUTHORIZED;
	drmMemory_t memory;
	uint8_t i;
	playbackContext_t *context = (playbackContext_t *) drmContext;
	uint16_t memorySerialNumber;
	uint16_t fileSerialNumber;
	drmErrorCodes_t finalResult = DRM_NOT_AUTHORIZED;

    if (   ( useLimit == NULL )
        || ( useCount == NULL )
        || ( rentalMessageFlag == NULL )
        || ( drmContext == NULL ) )
    {
		return DRM_GENERAL_ERROR;
    }

	*rentalMessageFlag = DIVX_FALSE;
	*useLimit = 0;
	*useCount = 0;

	if ( DRM_STATE_INITIALIZED != context->state )
	{
		setLastError( drmContext,
		              (drmErrorCodes_t)DRM_ERROR_NOT_INITIALIZED );
		return finalResult;
	}

	memset( &memory,
            0,
            sizeof(drmMemory_t) );

	context->state = DRM_STATE_RENTAL_QUERIED;

	if ( !(   DRM_TYPE_RENTAL == context->targetHeader.drmMode
           || DRM_TYPE_ACTIVATION_RENTAL == context->targetHeader.drmMode ) )

	{
		finalResult = DRM_SUCCESS;
		return finalResult;
	}

    returnCode = s_loadDrmMemory( &memory );

    if ( DRM_SUCCESS == returnCode )
    {
		/* Check if correct user, only if not activation record. */
		if (   ( 0 != memcmp(context->targetHeader.userId, memory.owner.userId, OWNER_USER_ID_BYTES) )
            && (   DRM_TYPE_RENTAL == context->targetHeader.drmMode
                || DRM_TYPE_PURCHASE == context->targetHeader.drmMode ) )
		{
			setLastError( drmContext,
		                  (drmErrorCodes_t)DRM_ERROR_NOT_AUTH_USER );
			return finalResult;
		}

		/* Ensure slot number is in range. */
		if ( context->targetHeader.rentalRecord.slotNumber >= TOTAL_PLAY_SLOTS )
		{
			setLastError( drmContext,
		                  (drmErrorCodes_t)DRM_ERROR_BAD_SLOT_NUMBER );
			return finalResult;
		}

		/* Find use limit id. */
		for ( i = 0; i < USE_LIMITS; i++ )
		{
			if ( useLimitIdInfo[i].id == context->targetHeader.rentalRecord.useLimitId )
			{
				*useLimit = useLimitIdInfo[i].uses;
				break;
			}
		}

		memorySerialNumber = encodingPack( memory.slots[context->targetHeader.rentalRecord.slotNumber].serialNumber[0],
								           memory.slots[context->targetHeader.rentalRecord.slotNumber].serialNumber[1] );
		fileSerialNumber = encodingPack( context->targetHeader.rentalRecord.serialNumber[0],
								         context->targetHeader.rentalRecord.serialNumber[1] );

		/* Check if correct serial number if the current activation file is different than the currently activated
		   Indicates activation file, matched explicit guard (i.e. already activated), slot initialized, do a check! */
		memcpy(guard, context->targetHeader.activateRecord.explicitGuard, OWNER_GUARD_FILE_BYTES);
        guard[3] =    context->targetHeader.guardExt.explicitGuardExt;
        if (   (   ( DRM_TYPE_ACTIVATION_RENTAL == context->targetHeader.drmMode )
                && ( 0 == memcmp(guard, memory.owner.guard, OWNER_GUARD_DMEM_BYTES ) )
                && ( 0 != memorySerialNumber ) )
            || ( DRM_TYPE_RENTAL == context->targetHeader.drmMode ) )
                
		{
			if ( fileSerialNumber < memorySerialNumber )
			{
				finalResult = DRM_RENTAL_EXPIRED;
				return finalResult;
			}

		    if (   ( fileSerialNumber == memorySerialNumber )
		        && ( USE_LIMIT_ID_UNLIMITED != context->targetHeader.rentalRecord.useLimitId ) )
		    {
			    *useCount = memory.slots[context->targetHeader.rentalRecord.slotNumber].counter;
			    if ( *useCount >= *useLimit )
			    {
				    finalResult = DRM_RENTAL_EXPIRED;
				    return finalResult;
			    }
		    }
		    
		    /* If a first time, uses will be zero. */
		    if ( fileSerialNumber > memorySerialNumber )
		    {
			    *useCount = 0;
		    }
			
		}
		
	    if ( USE_LIMIT_ID_UNLIMITED == context->targetHeader.rentalRecord.useLimitId )
	    {
	    	finalResult = DRM_SUCCESS;
		    return finalResult;
	    }

        *rentalMessageFlag = DIVX_TRUE;
		finalResult = DRM_SUCCESS;

    }

    return finalResult;
}

/*
 * 
 */
drmErrorCodes_t drmQueryCgmsa( uint8_t *drmContext,
                               uint8_t *cgmsaSignal )
{
	drmErrorCodes_t result = DRM_NOT_AUTHORIZED;
	playbackContext_t *context = (playbackContext_t *) drmContext;

    if (   ( context == NULL )
        || ( cgmsaSignal == NULL ) )
    {
		return DRM_GENERAL_ERROR;
    }

	if ( DRM_STATE_RENTAL_QUERIED == context->state )
	{
		if ( 1 == s_maskCheck( context->targetHeader.optionFlags,
                               OUTPUT_PROTECTION_MASK ) )
		{
			*cgmsaSignal = context->targetHeader.outputProtectionFlags >> 
				( ACPTB_BITS + DIGITAL_PROTECTION_BITS + OUTPUT_SIGNAL_RESERVED_BITS );
		}
		else
		{
			*cgmsaSignal = 0;
		}
		context->state = DRM_STATE_CGMSA_QUERIED;
		result = DRM_SUCCESS;
	}
	else
	{
		setLastError( drmContext,
		              (drmErrorCodes_t)DRM_ERROR_NOT_RENTAL_QUERIED );
	}

	return result;
}

/*
 * 
 */
drmErrorCodes_t drmQueryAcptb( uint8_t *drmContext,
                               uint8_t *acptbSignal )
{
	drmErrorCodes_t result = DRM_NOT_AUTHORIZED;
	playbackContext_t *context = (playbackContext_t *) drmContext;
	
    if (   ( drmContext == NULL )
        || ( acptbSignal == NULL ) )
    {
		return DRM_GENERAL_ERROR;
    }

	if ( DRM_STATE_CGMSA_QUERIED == context->state )
	{
		if ( 1 == s_maskCheck( context->targetHeader.optionFlags,
                               OUTPUT_PROTECTION_MASK ) )
		{
			*acptbSignal = context->targetHeader.outputProtectionFlags >> (DIGITAL_PROTECTION_BITS + OUTPUT_SIGNAL_RESERVED_BITS);
			*acptbSignal = ((0xFF << ACPTB_BITS) ^ (0xFF)) & (*acptbSignal);  // Zero out other bits that are not used.
		}
		else
		{
			*acptbSignal = 0;
		}
		context->state = DRM_STATE_ACPTB_QUERIED;
		result = DRM_SUCCESS;
	}
	else
	{
		setLastError( drmContext,
		              (drmErrorCodes_t)DRM_ERROR_NOT_CGMSA_QUERIED );
	}

	return result;
}

/*
 * 
 */
drmErrorCodes_t drmQueryDigitalProtection( uint8_t *drmContext,
                                           uint8_t *digitalProtectionSignal )
{
	drmErrorCodes_t result = DRM_NOT_AUTHORIZED;
	playbackContext_t *context = (playbackContext_t *) drmContext;

    if (   ( drmContext == NULL )
        || ( digitalProtectionSignal == NULL ) )
    {
		return DRM_GENERAL_ERROR;
    }

	if ( DRM_STATE_ACPTB_QUERIED == context->state )
	{
		if ( 1 == s_maskCheck( context->targetHeader.optionFlags,
                               OUTPUT_PROTECTION_MASK ) )
		{
			*digitalProtectionSignal = context->targetHeader.outputProtectionFlags >>  (OUTPUT_SIGNAL_RESERVED_BITS);
			*digitalProtectionSignal = ((0xFF << DIGITAL_PROTECTION_BITS) ^ (0xFF)) & (*digitalProtectionSignal);  // Zero out other bits that are not used.
		}
		else
		{
			*digitalProtectionSignal = 0;
		}
		context->state = DRM_STATE_DIGITAL_PROTECTION_QUERIED;
		result = DRM_SUCCESS;
	}
	else
	{
		setLastError( drmContext,
		              (drmErrorCodes_t)DRM_ERROR_NOT_ACPTB_QUERIED );
	}

	return result;
}

/*
 * 
 */
drmErrorCodes_t drmCommitPlayback( uint8_t *drmContext )
{
	uint8_t sessionKey[KEY_SIZE_BYTES] = {0};
	/* int decryptedFrameKeySize = 0; */
	playbackContext_t *context = (playbackContext_t *) drmContext;
	drmErrorCodes_t result = DRM_SUCCESS;
	/* int32_t cryptoResult = DIVX_TRUE; */

	if ( NULL == drmContext )
	{
		return DRM_GENERAL_ERROR;
	}    

	if ( DRM_STATE_DIGITAL_PROTECTION_QUERIED != context->state )
	{
		setLastError( drmContext,
		              (drmErrorCodes_t)DRM_ERROR_NOT_DIGITAL_QUERIED );
		return DRM_NOT_AUTHORIZED;
	}

	/* Try to get the session key. */
	result = s_getSessionKey( context,
                              sessionKey );
	if ( DRM_SUCCESS != result )
	{
		return DRM_NOT_AUTHORIZED; /* Trust setLastError from getSession was set. */
	}
	
    /* Load transaction information to scroll to the frame keys */
    {
        mem2TransactionHeader( &context->strdPtr,
                               &context->transactionHeader,
                               0 );
    }

    /* Populate the frame key table. */
    {
    	aesKeyHandle sessionKeyHandle;
        if ( 0 == DivXAesCreateKey ( &sessionKeyHandle,
                                     aesKeySize16Bytes,
                                     sessionKey ) )
        {
            mem2FrameKeys( &context->strdPtr,
                           &context->frameKeys,
                           sessionKeyHandle );
	        context->state = DRM_STATE_COMMITTED;
            memset( sessionKey,
                    0xCC,
                    sizeof(sessionKey) );
            DivXAesDeleteKey( sessionKeyHandle );
        }
        else
        {
            memset( sessionKey,
                    0xCC,
                    sizeof(sessionKey) );
	        setLastError( drmContext,
		                  (drmErrorCodes_t)DRM_ERROR_DECRYPTION_FAILED );
	        return DRM_NOT_AUTHORIZED;
        }

    }
    
    /* Populate the frame keys for decrypt */
    {
        int32_t i = 0;
        int32_t returnCode = 0;
        for ( i = 0; i < VIDEO_KEY_COUNT_MAX; i++ )
        {
            returnCode = DivXAesCreateKey ( &context->keys[i],
                                            aesKeySize16Bytes,
                                            context->frameKeys.frameKeys[i] );
            if ( returnCode != 0 )
            {
                return DRM_NOT_AUTHORIZED;
            }
        }

        return DRM_SUCCESS;
    }

    /* statement is unreachable */
    /* return DRM_NOT_AUTHORIZED; */

}

/*
 * 
 */
drmErrorCodes_t drmFinalizePlayback( uint8_t *drmContext )
{
    int i = 0;
	playbackContext_t *context = (playbackContext_t *) drmContext;
	
    if ( drmContext == NULL )
    {
		return DRM_GENERAL_ERROR;
    }

    for ( i = 0; i < sizeof(context->keys)/sizeof(aesKeyHandle); i++ )
    {
        if ( context->keys[i] != 0 )
        {
            DivXAesDeleteKey ( context->keys[i] );
            context->keys[i] = 0;
        }
    }
    DivXPortableFinalize( );
    
    context->state = DRM_STATE_BOOT;
    return DRM_SUCCESS;
}

#if 1
drmErrorCodes_t drmQueryFrameKeys(uint8_t* drmContext, uint8_t* frame_keys_count, uint8_t* frame_keys)
{
    drmErrorCodes_t result = DRM_NOT_AUTHORIZED;
    playbackContext_t *context = (playbackContext_t *) drmContext;

    if ( ( drmContext == NULL ) || ( frame_keys_count == NULL )  || ( frame_keys == NULL) )
    {
        return DRM_GENERAL_ERROR;
    }

    if (DRM_STATE_COMMITTED == context->state)
    {
        *frame_keys_count = context->targetHeader.frameKeyCount;
        memcpy(frame_keys, context->frameKeys.frameKeys, (uint8_t)(*frame_keys_count)*VIDEO_KEY_SIZE_BYTES);
                    
        context->state = DRM_STATE_FRAME_KEY_QUERIED;
        result = DRM_SUCCESS;
    }
    else
    {
        setLastError( drmContext, (drmErrorCodes_t)DRM_ERROR_NOT_COMMITTED);
    }
        
    return result;
}

drmErrorCodes_t drmQueryAudioProtection(uint8_t* drmContext, audioCryptoInfo_t* audioCryptoInfo)
{
    drmErrorCodes_t result = DRM_NOT_AUTHORIZED;
    playbackContext_t *context = (playbackContext_t *) drmContext;

    if (DRM_STATE_FRAME_KEY_QUERIED == context->state)
    {
        if (1 == s_maskCheck( context->targetHeader.optionFlags, PROTECTED_AUDIO_MASK))
        {
            audioCryptoInfo->flag = PROTECTED_AUDIO;
            memcpy(audioCryptoInfo->audioKey, context->frameKeys.frameKeys[AUDIO_USE_VIDEO_KEY_INDEX], AUDIO_KEY_SIZE_BYTES);
            audioCryptoInfo->offset = context->targetHeader.protectedAudioOffset;
            audioCryptoInfo->size   = context->targetHeader.protectedAudioCryptoSize;
        }
        else
        {
            audioCryptoInfo->flag = 0;
        }
        context->state = DRM_STATE_AUDIO_PROTECTION_QUERIED;
        /* for play back again without mopen */
        /* context->state = DRM_STATE_DIGITAL_PROTECTION_QUERIED; */
        result = DRM_SUCCESS;
    }
    else
    {
        setLastError(drmContext, (drmErrorCodes_t)DRM_ERROR_NOT_KEY_QUERIED);
    }

    return result;
}

drmErrorCodes_t drmIncreaseRental(playbackContext_t *drmContext)
{
    uint8_t j;
    uint8_t returnCode = DRM_SUCCESS;
    uint8_t useLimit = 255;  /* Ensure use limit initialized to max for correct security logic. */
    drmTargetHeader_t fileInfo = drmContext->targetHeader;    

    if (DRM_STATE_AUDIO_PROTECTION_QUERIED != drmContext->state)
    {
        setLastError((uint8_t *)drmContext, (drmErrorCodes_t)DRM_ERROR_NOT_AUDIO_QUERIED);
        return DRM_NOT_AUTHORIZED;
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
        if ( (useLimit > drmContext->t_drm_memory.slots[fileInfo.rentalRecord.slotNumber].counter) ||
             (USE_LIMIT_ID_UNLIMITED == fileInfo.rentalRecord.useLimitId) )
        {    
            /* Update counter, yes still does if unlimited.  This is ok. */
            drmContext->t_drm_memory.slots[fileInfo.rentalRecord.slotNumber].counter++;
        }
        else
        {
            setLastError((uint8_t *)drmContext, (drmErrorCodes_t)DRM_ERROR_RENTAL_EXPIRED);
            return DRM_RENTAL_EXPIRED;
        }
    }

    return (drmErrorCodes_t)returnCode;
}
#endif

/*
 * 
 */
drmErrorCodes_t drmDecryptVideo( uint8_t *drmContext,
                                 uint8_t *frame,
                                 uint32_t frameSize,
                                 uint8_t *drmFrameInfo )
{
	playbackContext_t *context = (playbackContext_t *) drmContext;
	drmErrorCodes_t result = DRM_NOT_AUTHORIZED;
    int32_t decryptedFrameSize = 0;
    /* int32_t cryptoResult = DIVX_TRUE; */
    drmVideoDDChunk_t videoDDChunk;
        
	if ( DRM_STATE_COMMITTED == context->state )
	{
        uint8_t* ddChunk = drmFrameInfo;
        mem2VideoDDChunk( &ddChunk,
                          &videoDDChunk,
                          0 );

        decryptedFrameSize = videoDDChunk.size;
        DivXAesECBDecrypt( context->keys[videoDDChunk.keyIndex],
                           frame + videoDDChunk.offset,
                           videoDDChunk.size,
                           frame + videoDDChunk.offset,
                           (uint32_t *)&decryptedFrameSize );
        if ( decryptedFrameSize != (int32_t)videoDDChunk.size)
        {
            setLastError( drmContext,
		                  (drmErrorCodes_t)DRM_ERROR_DECRYPTION_FAILED );
        }
        else
        {
		    result = DRM_SUCCESS;
        }
	}
	else
	{
		setLastError( drmContext,
		              (drmErrorCodes_t)DRM_ERROR_NOT_COMMITTED );
	}

	return result;
}

/*
 * 
 */
drmErrorCodes_t drmDecryptAudio( uint8_t *drmContext,
                                 uint8_t *frame,
                                 uint32_t frameSize )
{
	playbackContext_t *context = (playbackContext_t *) drmContext;
	drmErrorCodes_t result = DRM_NOT_AUTHORIZED;
	audioCryptoInfo_t audioSettings;

	if ( DRM_STATE_COMMITTED == context->state )
	{
		if ( 0 == s_maskCheck( context->targetHeader.optionFlags,
                               PROTECTED_AUDIO_MASK ) )
		{
			return DRM_SUCCESS; // Ignore if no crypto.
		}

		audioSettings.key = context->keys[AUDIO_USE_VIDEO_KEY_INDEX];
		audioSettings.offset = context->targetHeader.protectedAudioOffset;
		audioSettings.size = context->targetHeader.protectedAudioCryptoSize;

		if ( s_decryptAudio( &audioSettings,
                                     frame,
                                     frameSize ) == 0 )
		{
			result = DRM_SUCCESS;
		}
		else
		{
			setLastError( drmContext,
		                  (drmErrorCodes_t)DRM_ERROR_DECRYPTION_FAILED );
		}

		if ( DRM_SUCCESS != result )
		{
			setLastError( drmContext,
		                  (drmErrorCodes_t)DRM_ERROR_DECRYPTION_FAILED );
		}
	}
	else
	{
		setLastError( drmContext,
		              (drmErrorCodes_t)DRM_ERROR_NOT_COMMITTED );
	}

	return result;
}
 
/*
 * 
 */
#if 0
int drmGetVersion( DivXVersion *pVersion )
{

    /*! Static string to define the library name returned by GetVersion() */
    static const char official[] = "DivXDrmApi";
    
    /*! Build label substituted at built time for proper library version */
    static const char buildLabel[] = DIVXVERSION_LABEL_PLACEHOLDER;
    
    if ( pVersion == NULL )
    {
        return DIVXVERSION_INVALID_PARAM;
    }

    pVersion->officialName = official;
    if ( sscanf( buildLabel,
                 "%d_%d_%d_%d",
                 &pVersion->major,
                 &pVersion->minor,
                 &pVersion->fix,
                 &pVersion->build ) != 4 )
    {
        return DIVXVERSION_INVALID_LABEL;
    }

    pVersion->codeName = strrchr( buildLabel,
                                  DIVXVERSION_LABEL_SEPARATOR );
    if ( pVersion->codeName )
    {
        pVersion->codeName = &( pVersion->codeName[ 1 ] );
    }
    
    return DIVXVERSION_OK;
}
#endif


/*-----------------------------------------------------------------------------------------------------
//
//  STATIC FUNCTIONS
//
//----------------------------------------------------------------------------------------------------*/

/*
 * 
 */
drmErrorCodes_t s_randomizeGuard( playbackContext_t *context,
                                  drmMemory_t *memory )
{
    uint8_t i;
	uint8_t randomByte;
	drmErrorCodes_t returnCode = DRM_SUCCESS;


	memcpy( memory->owner.guard,
            nullGuard,
            OWNER_GUARD_DMEM_BYTES );

    memcpy( memory->owner.userId,
            initUserId,
            OWNER_USER_ID_BYTES );

	for ( i = 0; i < OWNER_GUARD_DMEM_BYTES; i++ )
	{
		if ( context->randomSamples > 0 )
		{
			randomByte = context->randomByteBucket[context->randomSamples];
			context->randomSamples--;
		}
		else
		{
			setLastError( (uint8_t*)context,
		                  (drmErrorCodes_t)DRM_ERROR_NEED_MORE_RANDOM_SAMPLE );
			return DRM_NOT_AUTHORIZED;
		}

		memory->owner.guard[i] = randomByte;
	}

	/* Ensure guard cannot be null guard.  This should be a 2^24 chance. */
	if ( 0 == memcmp( memory->owner.guard,
                      nullGuard,
                      OWNER_GUARD_DMEM_BYTES ) )
	{
		memory->owner.guard[0] = 1;
	}
	
	return returnCode;
}

/*
 * This function is a bit long. Careful, it has multiple return statements.
 */
drmErrorCodes_t s_getSessionKey( playbackContext_t *context,
                                 uint8_t *sessionKey )
{
    uint8_t guard[OWNER_GUARD_DMEM_BYTES] = {0};
    drmErrorCodes_t returnCode = DRM_SUCCESS;
    uint8_t i = 0;
	uint8_t j = 0;
    uint8_t useLimit = 255;  /* Ensure use limit initialized to max for correct security logic. */
	drmMemory_t memory = {0};
	uint8_t clearKey[KEY_SIZE_BYTES] = {0};
	uint16_t fileSerialNumber = 0;
	uint16_t memorySerialNumber = 1;
	uint8_t userKeyMask[KEY_SIZE_BYTES] =
	{
	    118, 151,  74, 140, 206, 221, 122, 171,
		122,  86, 188, 103,   0,   52, 55,  76
    };
	drmTargetHeader_t fileInfo = context->targetHeader;

	if ( DRM_TYPE_BASE == fileInfo.drmMode )
	{
		memcpy( sessionKey,
                fileInfo.sessionKey,
                KEY_SIZE_BYTES );
		return DRM_SUCCESS;
	}

	returnCode = s_loadDrmMemory( &memory );
    if ( DRM_SUCCESS != returnCode )
    {
		setLastError( (uint8_t*)context,
		              returnCode );
        return DRM_NOT_AUTHORIZED;
    }  

	/* Check if we need to activate.  Only activate signed files. */
	if ((   ( DRM_TYPE_ACTIVATION_PURCHASE == fileInfo.drmMode )
        || ( DRM_TYPE_ACTIVATION_RENTAL == fileInfo.drmMode ) )&&
        s_maskCheck(fileInfo.optionFlags, SIGNED_ACTIVATION_MASK))/*must be a signed activation*/
	{

	    /* Create a local copy of the device's modelId for determining if we are activating the correct model. */
	    uint8_t modelId[MODEL_ID_BYTES];
	    drmDataLoadModelId( modelId );
    
        memcpy(guard, fileInfo.activateRecord.memoryGuard, OWNER_GUARD_FILE_BYTES);
        guard[3] = fileInfo.guardExt.memoryGuardExt;
		/* Check guard and model id. */
		if (   ( 0 == memcmp( guard,
                              memory.owner.guard,
                              OWNER_GUARD_DMEM_BYTES ) ) 
            && ( 0 == memcmp( fileInfo.activateRecord.modelId,
                              modelId,
                              MODEL_ID_BYTES ) ) )
		{

			/* Check to see if we are being tested for initial guard of zero.  If so, return. */
			if ( 0 == memcmp( nullGuard,
                              memory.owner.guard,
                              OWNER_GUARD_DMEM_BYTES ) )
			{
				setLastError( (uint8_t*)context,
		                      (drmErrorCodes_t)DRM_ERROR_NULL_GUARD_SET_SIGNAL );
                returnCode = DRM_NOT_AUTHORIZED;
                goto s_getSessionKey_ACTERROR;
			}

            memcpy(guard, fileInfo.activateRecord.explicitGuard, OWNER_GUARD_FILE_BYTES);
            guard[3] = fileInfo.guardExt.explicitGuardExt;

			/* Reset guard for next activation. */
			if ( 0 == memcmp( guard,
                              nullGuard,
                              OWNER_GUARD_DMEM_BYTES ) )
			{
				/* Was set zero in file this is not allowed. Server must always be issuing
				   a guard. */
				setLastError( (uint8_t*)context,
		                      returnCode );
                returnCode = DRM_NOT_AUTHORIZED;
                goto s_getSessionKey_ACTERROR;
			}
			else
			{
				/* Explicitly set the guard for next activation. */
				for ( i = 0; i < OWNER_GUARD_FILE_BYTES; i++ )
				{
					memory.owner.guard[i] = fileInfo.activateRecord.explicitGuard[i];
				}
                memory.owner.guard[i] = fileInfo.guardExt.explicitGuardExt;
			}
			
			/* Handle user key.  Xor record key before setting. */
			for ( i = 0; i < KEY_SIZE_BYTES; i++ )
			{
				clearKey[i] = fileInfo.activateRecord.userKey[i] ^ userKeyMask[i];
			}
			memcpy( memory.owner.key,
                    clearKey,
                    KEY_SIZE_BYTES );
			memset( clearKey,
                    0,
                    KEY_SIZE_BYTES );

			/* User id. */
			memcpy( memory.owner.userId,
                    fileInfo.userId,
                    OWNER_USER_ID_BYTES );
			
			/* Reset slots. */
			for ( i = 0; i < TOTAL_PLAY_SLOTS; i++ )
			{
				memory.slots[i].counter = 0;
				memset( memory.slots[i].serialNumber,
                        0,
                        SLOT_SERIAL_NUMBER_BYTES );
			}
#if DRM_OTHER_SECURE_PLATFORM == 0
			memory.activationFailureCnt = 0;
#endif
#if 0
			returnCode = s_saveDrmMemory( &memory );
			if ( DRM_SUCCESS != returnCode )
			{
				setLastError( (uint8_t*)context,
		                      returnCode );
				return DRM_NOT_AUTHORIZED;
			}
#else
                        context->b_save_drm_memory = DIVX_TRUE;
                        memcpy((void *)&(context->t_drm_memory), &memory, sizeof(drmMemory_t));
#endif
		}
        else
        {
		    if( memcmp( guard, memory.owner.guard, OWNER_GUARD_DMEM_BYTES ) != 0) 
            {
    			setLastError( (uint8_t*)context, (drmErrorCodes_t)DRM_ERROR_GUARD_MISMATCH );
            }
            if( memcmp( fileInfo.activateRecord.modelId, modelId, MODEL_ID_BYTES ) != 0) 
            {
    			setLastError( (uint8_t*)context, (drmErrorCodes_t)DRM_ERROR_MODEL_MISMATCH );
            }
            
        }
	}
    else
    {
        if((  (DRM_TYPE_ACTIVATION_PURCHASE == fileInfo.drmMode ) ||
             ( DRM_TYPE_ACTIVATION_RENTAL == fileInfo.drmMode  ) )&&
            (s_maskCheck(fileInfo.optionFlags, SIGNED_ACTIVATION_MASK) == 0) )
        {
  			setLastError( (uint8_t*)context, (drmErrorCodes_t)DRM_ERROR_NO_SIGNATURE );
        }

    }

    /* Purchase logic. */
    if (   DRM_TYPE_PURCHASE == fileInfo.drmMode
        || DRM_TYPE_ACTIVATION_PURCHASE == fileInfo.drmMode )
    {        
		if ( 0 == memcmp( fileInfo.userId,
                          memory.owner.userId,
                          OWNER_USER_ID_BYTES ) )
		{
			for ( i = 0; i < KEY_SIZE_BYTES; i++ )
			{
				sessionKey[i] = fileInfo.sessionKey[i] ^ memory.owner.key[i];
			} 
		}
		else
		{
			setLastError( (uint8_t*)context,
		                  (drmErrorCodes_t)DRM_ERROR_NOT_AUTH_USER );
			return DRM_NOT_AUTHORIZED;
		}
    }

	/* Rental logic. */
	if (   DRM_TYPE_RENTAL == fileInfo.drmMode
        || DRM_TYPE_ACTIVATION_RENTAL == fileInfo.drmMode )
	{
		/* Ensure correct user. */
		if ( 0 == memcmp( fileInfo.userId,
                          memory.owner.userId,
                          OWNER_USER_ID_BYTES ) )
		{
			// Ensure slot number is in range.
			if ( fileInfo.rentalRecord.slotNumber >= TOTAL_PLAY_SLOTS )
			{
				setLastError( (uint8_t*)context,
		                      (drmErrorCodes_t)DRM_ERROR_BAD_SLOT_NUMBER );

				return DRM_NOT_AUTHORIZED;
			}

			// Put serial number in form we can compare to.
			memorySerialNumber = encodingPack( memory.slots[fileInfo.rentalRecord.slotNumber].serialNumber[0],
                                               memory.slots[fileInfo.rentalRecord.slotNumber].serialNumber[1] );
            fileSerialNumber = encodingPack( fileInfo.rentalRecord.serialNumber[0],
									         fileInfo.rentalRecord.serialNumber[1] );

			// Check first serial number.
			if ( fileSerialNumber > memorySerialNumber )
			{
				// Update serial number.
				memcpy( memory.slots[fileInfo.rentalRecord.slotNumber].serialNumber,
					    fileInfo.rentalRecord.serialNumber,
                        SLOT_SERIAL_NUMBER_BYTES );
				memorySerialNumber = fileSerialNumber;

				// Reset counter.
				memory.slots[fileInfo.rentalRecord.slotNumber].counter = 0;
			}

			// Check subsequent serial number.
			if ( fileSerialNumber == memorySerialNumber )
			{
				// Find use limit id.
				for ( j = 0; j < USE_LIMITS; j++ )
				{
					if ( useLimitIdInfo[j].id == fileInfo.rentalRecord.useLimitId )
					{
						useLimit = useLimitIdInfo[j].uses;
						break;
					}
				}

				// Check use limit id.
				if (   ( useLimit > memory.slots[fileInfo.rentalRecord.slotNumber].counter )
                    || ( USE_LIMIT_ID_UNLIMITED == fileInfo.rentalRecord.useLimitId ) )
				{
				        #if 0
					// Update counter, yes still does if unlimited.  This is ok.
					memory.slots[fileInfo.rentalRecord.slotNumber].counter++;
                                        #endif

					// And finally, copy over the key.
					for ( i = 0; i < KEY_SIZE_BYTES; i++ )
					{
						sessionKey[i] = fileInfo.sessionKey[i] ^ memory.owner.key[i];
					} 
				}
				else
				{
					setLastError( (uint8_t*)context,
		                          (drmErrorCodes_t)DRM_ERROR_RENTAL_EXPIRED );
					return DRM_NOT_AUTHORIZED;
				}
			}
			else
			{
				setLastError( (uint8_t*)context,
		                      (drmErrorCodes_t)DRM_ERROR_RENTAL_EXPIRED );
				return DRM_NOT_AUTHORIZED;
			}
		}
		else
		{
			setLastError( (uint8_t*)context,
		                  (drmErrorCodes_t)DRM_ERROR_NOT_AUTH_USER );
			return DRM_NOT_AUTHORIZED;
		}
	}

    
    if ( DRM_SUCCESS == returnCode )
    {
#if 0
        returnCode = s_saveDrmMemory( &memory );
		if ( DRM_SUCCESS != returnCode )
		{
			setLastError( (uint8_t*)context,
		                  returnCode );
			returnCode = DRM_NOT_AUTHORIZED;
		}
#else
        context->b_save_drm_memory = DIVX_TRUE;
        memcpy((void *)&(context->t_drm_memory), &memory, sizeof(drmMemory_t));
#endif
    }
    
    return returnCode;

s_getSessionKey_ACTERROR:

	/* Timer delay to prevent programmatic retry of activation. */
#if DRM_OTHER_SECURE_PLATFORM == 0	
	memory.activationFailureCnt++;
#endif
    returnCode = s_saveDrmMemory( &memory );
    
    /* In the even that the attacker has deleted, removed, or otherwise prevented the writing of DRM memory,
       push a value regardless */
#if DRM_OTHER_SECURE_PLATFORM == 0	
    if ( returnCode != 0 )
    {
        memory.activationFailureCnt = 1;
    }
    {
        uint32_t tick = localClock( );
        uint32_t expire = tick + ( localClockTicksPerSec( ) * memory.activationFailureCnt );
		
        while ( tick < expire )
        {
            tick = localClock( );
        }
    }
#else
    {
        uint32_t tick = localClock( );
        uint32_t expire = tick + localClockTicksPerSec( );
		
        while ( tick < expire )
        {
            tick = localClock( );
        }
    }
#endif

    return (drmErrorCodes_t)DRM_NOT_AUTHORIZED;

}

uint8_t s_maskCheck( const uint8_t option,
                     const uint8_t mask )
{
	uint8_t result = 0; // 0 = false, 1 = true.
	uint8_t check;

	check = option & mask;
	if ( mask == check )
	{
		result = 1;
	}
	
	return result;
}

int32_t s_decryptAudio( audioCryptoInfo_t *audioSettings,
                        uint8_t *frame,
                        uint32_t frameSize )
{
	int32_t result = -1;
	uint32_t processed;

	if ( frameSize < audioSettings->offset )
	{
		return 0;  // Ignore small frames.
	}

	if ( ( frameSize - audioSettings->offset ) < audioSettings->size)
	{
		return 0;  // Ignore small frames.
	}

	if ( 0 == audioSettings->size )
	{
		return 0; // Ignore if no crypto.
	}

    processed = audioSettings->size;
	DivXAesECBDecrypt( audioSettings->key,
                       frame + audioSettings->offset, 
                       audioSettings->size,
                       frame + audioSettings->offset, 
                       &processed );
	if ( (uint32_t)audioSettings->size == processed )
	{
		result = 0;
	}
	
	return result;
}

/*
 * 
 */
drmErrorCodes_t s_loadDrmMemory( drmMemory_t *memory )
{
	drmPackedMemory_t memoryPacked = {0};
	uint8_t* buffer = memoryPacked.packed;
    int32_t returnCode = 0;
#if DRM_OTHER_SECURE_PLATFORM == 0    
    uint8_t pad[4] = { 0xA5, 0xA5, 0xA5, 0xA5 };
#endif

    returnCode = drmDataLoadLoadDrmMemory( memoryPacked.packed,
                                           sizeof(drmPackedMemory_t) );
    if ( 0 == returnCode )
    {

       mem2DrmMemory( &buffer,
                       memory,
                       0 );
#if DRM_OTHER_SECURE_PLATFORM == 0
        if (   ( memcmp( memory->prefixPad,
                         pad,
                         sizeof(pad) ) != 0 )
            || ( memcmp( memory->postfixPad,
                         pad,
                         sizeof(pad) ) != 0 ) ) 
        {
		    returnCode = DRM_NOT_AUTHORIZED;
		    return (drmErrorCodes_t)returnCode;
        }
#endif
    }
	else
	{
		returnCode = DRM_NOT_AUTHORIZED;
		return (drmErrorCodes_t)returnCode;
	}
    
    return DRM_SUCCESS;
}

/*  s_saveDrmMemory
 * 
 */
drmErrorCodes_t s_saveDrmMemory( drmMemory_t *memory )
{
    drmPackedMemory_t memoryPacked = {0};
    uint8_t* buffer = memoryPacked.packed;    
    int32_t returnCode = 0;
#if DRM_OTHER_SECURE_PLATFORM == 0
    int32_t i = 0;
#endif

    /* Randomize the random pad in the memory so the crypt text is always moving. */
#if DRM_OTHER_SECURE_PLATFORM == 0
    DivXRngGetRandomBytes( memory->randomPad1,
                           sizeof(memory->randomPad1) );
    DivXRngGetRandomBytes( memory->randomPad2,
                           sizeof(memory->randomPad2) );
    DivXRngGetRandomBytes( &memory->owner.randomPad1,
                           sizeof(memory->owner.randomPad1) );
    DivXRngGetRandomBytes( &memory->owner.randomPad2,
                           sizeof(memory->owner.randomPad2) );
    DivXRngGetRandomBytes( &memory->owner.randomPad3,
                           sizeof(memory->owner.randomPad3) );
    for ( i = 0; i < TOTAL_PLAY_SLOTS; i++ )
    {
        DivXRngGetRandomBytes( &memory->slots[i].randomPad,
                               sizeof(memory->slots[i].randomPad) );
                               
    }
#endif

    /* Marshall the bytes from structure to memory */
    drmMemory2Mem( memory,
                   &buffer,
                   0 );

    returnCode = drmDataLoadSaveDrmMemory( memoryPacked.packed,
                                           sizeof(drmPackedMemory_t) );
    if ( 0 != returnCode )
    {
		returnCode = DRM_NOT_AUTHORIZED;
		return (drmErrorCodes_t)returnCode;
	}
    
    return DRM_SUCCESS;
}

int32_t s_activationGenerateProtectedMessage( uint8_t* messageBuffer,
                                              rsaPublicKeyHandle rsaPublicKey,
                                              int8_t* code,
                                              uint8_t* guard,
                                              uint8_t flags )
{
    drmActivationProtectedMessage_t protectedMessage = {0};
    drmActivationMessage_t message = {0};
    int32_t retVal = 0;
    uint32_t hashSize = SHA256_SIZE_BYTES;
    int32_t size = 0;
    uint8_t hashedBuffer[SHA256_SIZE_BYTES];
    uint8_t cryptoBuffer[DRM_ACTIVATION_MSG_PAYLOAD_BYTES];
    uint8_t clearBuffer[sizeof(drmActivationMessage_t)];

    /*populate my message*/
    message.flags[0] = 0x00;
    message.flags[1] = 0x00;
    message.flags[2] = 0x00;
    message.flags[3] = flags;
    
    retVal = DivXSha256( (uint8_t *)guard,
                         OWNER_GUARD_DMEM_BYTES,
                         hashedBuffer,
                         &hashSize );

    if ( retVal != 0 )
    {
        return -1;
    }
    memcpy( message.guardHash,
            hashedBuffer,
            hashSize );
           
    memcpy( message.registrationCode,
            code,
            sizeof(message.registrationCode) );

    memset( message.reserved,
            0x00,
            sizeof(message.reserved) );
    memset( message.a1Padding,
            0xA1,
            sizeof(message.a1Padding) );
    message.a1Padding[0] = 0x00;

    s_activationMsgStruct2Mem( clearBuffer,
                               &message );
       
    size = DRM_ACTIVATION_MSG_PAYLOAD_BYTES;
    DivXRsaPublicEncrypt( rsaPublicKey, 
                          clearBuffer,
                          DRM_ACTIVATION_MSG_PAYLOAD_BYTES,
                          cryptoBuffer,
                          &size );
      
    memcpy( protectedMessage.protectedPayload,
            cryptoBuffer,
            DRM_ACTIVATION_MSG_PAYLOAD_BYTES );
    protectedMessage.version = DRM_ACTIVATION_MESSAGE_VERSION;

    s_activationProtectedMsgStruct2Mem( messageBuffer,
                                        &protectedMessage );

    return 0;

}

static void s_activationMsgStruct2Mem( uint8_t* toBuffer,
                                       drmActivationMessage_t* fromMessage )
{
  	uint8_t *bufferPointer = toBuffer;

    memcpy( bufferPointer,
            fromMessage->a1Padding,
            sizeof(fromMessage->a1Padding) );
    bufferPointer += sizeof(fromMessage->a1Padding);

    memcpy( bufferPointer,
            fromMessage->flags,
            sizeof(fromMessage->flags) );
    bufferPointer += sizeof(fromMessage->flags);

    memcpy( bufferPointer,
            fromMessage->reserved,
            sizeof(uint8_t) * sizeof(fromMessage->reserved) );
    bufferPointer += sizeof(fromMessage->reserved);

    memcpy( bufferPointer,
            fromMessage->registrationCode,
            sizeof(fromMessage->registrationCode) );
    bufferPointer += sizeof(fromMessage->registrationCode);

    memcpy( bufferPointer,
            fromMessage->guardHash,
            sizeof(fromMessage->guardHash) );
    bufferPointer += sizeof(fromMessage->guardHash);

}


static void s_activationProtectedMsgStruct2Mem( uint8_t* toBuffer,
                                                drmActivationProtectedMessage_t* fromMessage )
{
  	uint8_t *bufferPointer = toBuffer;
  	
  	/* Copy in the version into the buffer */
    memcpy( bufferPointer,
            &fromMessage->version,
            sizeof(uint32_t) );
    bufferPointer += sizeof(uint32_t);

  	/* Concatenate the protected payload into the buffer */
    memcpy( bufferPointer,
            fromMessage->protectedPayload,
            sizeof(uint8_t) * DRM_ACTIVATION_MSG_PAYLOAD_BYTES );
    bufferPointer += sizeof(uint8_t) * DRM_ACTIVATION_MSG_PAYLOAD_BYTES;
    
}


/*
  the purpose of this function is to process the signature of
  the activation file.  the activation record is hashed, combined
  with a key and encrypted with RSA to create a digest.
*/
static uint8_t s_processSignature(playbackContext_t *context)
{
	drmDigest_t     digest = {0};
	uint8_t         fileHash[SHA256_SIZE_BYTES] = {0};
    aesKeyHandle    digestKeyInstance = {0};
    rsaPublicKeyHandle rsaPublicKey;
    uint8_t         digestKeyHash[SHA256_SIZE_BYTES] = {0};
	uint8_t         *activateRecordSubBlock = NULL;
    uint8_t         fullSignedData[256] = {0};
    uint8_t         targetHeaderMem[sizeof(drmTargetHeader_t)] = {0};
    uint8_t         *pTargetHdr = targetHeaderMem;
    uint8_t         activateRecordMem[sizeof(drmActivateRecord_t)] = {0};
    uint8_t         *pActivationRecord = activateRecordMem;
	/* int32_t         amountDecrypted = 0; */
    int32_t         i = 0;
    int32_t         returnCode = 0;
    uint32_t        size = 0;
    uint32_t        insize = 0;
    uint32_t        outsize = 0;
    /* uint32_t        digestKeyHashSize = 0; */
    uint8_t         result = DRM_SUCCESS;
    uint8_t         publicKeyE[2]  = {0};
    uint8_t         publicKeyN[256] = {0};
    char            nHex[1024] = {0};
    /* uint8_t         offset= 0; */

    targetHeader2Mem(&context->targetHeader, &pTargetHdr, 0);
    insize = sizeof(targetHeaderMem);
    outsize = sizeof(fileHash);
	returnCode = DivXSha256((uint8_t *) targetHeaderMem, insize, fileHash, &outsize);

    if(drmDataLoadGetPublicKey( publicKeyE,  publicKeyN ) == 0)
    {
        encodingConvertBinToHex(publicKeyN, sizeof(publicKeyN), nHex);
        if(DivXRsaCreatePublicKey( &rsaPublicKey,
                            publicKeyN,
                            (int32_t)sizeof(publicKeyN),
                            publicKeyE,
                            (int32_t)sizeof(publicKeyE) ) != 0)
        {
            return DRM_NOT_AUTHORIZED;
        }
        outsize = sizeof(fullSignedData);
        if(DivXRsaPublicDecrypt(rsaPublicKey, context->signature.signedData, sizeof(context->signature.signedData),
                                       fullSignedData, (int32_t *)&outsize) != 0)
        {
            return DRM_NOT_AUTHORIZED;
        }
        DivXRsaDeletePublicKey(rsaPublicKey);
    }
    else
    {
        return DRM_NOT_AUTHORIZED;
    }

	if ( 0 == fullSignedData[0] )  // Convention of encrypt.
	{
		memcpy(&digest, fullSignedData + 1, sizeof(drmDigest_t) - 1);  // Skip first byte, it was zero.
	}
    else
    {
        memcpy(&digest, fullSignedData, sizeof(drmDigest_t));  // Only get digest portion, rest is random data.
    }
    if(memcmp(digest.hash, fileHash, SHA256_SIZE_BYTES) == 0)
    {
        DivXAesCreateKey ( &digestKeyInstance, aesKeySize16Bytes, 
                        digest.key ) ;
        activateRecord2Mem(&(context->targetHeader.activateRecord), 
                        &pActivationRecord, 0);
        pActivationRecord = activateRecordMem;//these copy functions advance the mem pointers, so we must reset

        if(DivXAesECBDecrypt(digestKeyInstance, activateRecordMem, 
                        sizeof(drmActivateRecord_t),activateRecordMem, &size) != 0)
        {
            return DRM_NOT_AUTHORIZED;
        }
        DivXAesDeleteKey(digestKeyInstance);
        mem2ActivateRecord(&pActivationRecord,
                        &(context->targetHeader.activateRecord), 0);
        pActivationRecord = activateRecordMem;//these copy functions advance the mem pointers, so we must reset
        
    }
    activateRecord2Mem(&(context->targetHeader.activateRecord), &pActivationRecord, 0) ;  					
    pActivationRecord = activateRecordMem;//these copy functions advance the mem pointers, so we must reset
    activateRecordSubBlock = activateRecordMem + KEY_SIZE_BYTES;
    outsize = SHA256_SIZE_BYTES;
	returnCode = DivXSha256(digest.key, KEY_SIZE_BYTES, digestKeyHash, &outsize);
    if(returnCode != 0)
    {
        return DRM_NOT_AUTHORIZED;
    }
    /*undo the XOR*/
	for (i = 0; i < DRM_SIGNATURE_SUB_CRYPTO_BYTES; i++)
	{
		activateRecordSubBlock[i] = activateRecordSubBlock[i] ^ digestKeyHash[i];
	}
	mem2ActivateRecord(&pActivationRecord, &(context->targetHeader.activateRecord), 0);
    pActivationRecord = activateRecordMem;//these copy functions advance the mem pointers, so we must reset

	return result;
}
