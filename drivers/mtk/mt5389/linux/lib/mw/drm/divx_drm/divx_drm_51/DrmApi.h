/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/DrmApi.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

/*
These functions are the top level interface to the
Digital Rights Management (DRM) capabilities.
*/

/** @defgroup DRM DRM API
 *  Description of Digital Rights Management (DRM)
 *  @{
 */

#ifndef DRMAPI_H_INCLUDE
#define DRMAPI_H_INCLUDE

#ifndef __KERNEL__

#include "DrmTypes.h"
#include "crypto/DivXAesDecrypt.h"

#ifdef _MTK_BUILD_
#include "divx_drm_adp_common.h"
#endif 

#else

#include "DrmTypes.h"
#include "crypto/DivXAesDecrypt.h"

#endif

#if 0
#include "DivXVersion.h"
#endif

// Return values.
typedef enum drmErrorCodes 
{
    DRM_SUCCESS = 0,
    DRM_NOT_AUTHORIZED,
    DRM_NOT_REGISTERED,
    DRM_RENTAL_EXPIRED,
    DRM_GENERAL_ERROR,
} drmErrorCodes_t;

#if 1 /* Move from DrmApi.c */
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
#if 1    
    DRM_STATE_FRAME_KEY_QUERIED,
    DRM_STATE_AUDIO_PROTECTION_QUERIED,
#endif
    DRM_STATE_FINALIZED
} drmApiState_t;

#define RANDOM_BYTE_BUCKET_SIZE			8

#define AUDIO_KEY_SIZE_BYTES				16
#define AUDIO_CRYPTO_BLOCK_SIZE_BYTES		16
#define AUDIO_KEY_SIZE_BITS					128
#define AUDIO_MAX_ENCRYPT_BYTES             128
#define AUDIO_USE_VIDEO_KEY_INDEX           0

typedef struct audioCryptoInfo
{
    aesKeyHandle key;
#if 1
#define PROTECTED_AUDIO 1
    uint8_t flag;
    uint8_t audioKey[AUDIO_KEY_SIZE_BYTES];
#endif
	uint16_t offset;
	uint16_t size;
} audioCryptoInfo_t;

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
#if 1    
    DivXBool b_save_drm_memory;
    drmMemory_t t_drm_memory;    
#endif
} playbackContext_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @brief @b drmGetDrmContextSize returns number of bytes of context.

@return uint32_t
@retval Number of bytes of the context.

@b Note: This should be same as DRM_CONTEXT_SIZE_BYTES.  This function can provide a check during
development that the implementation of the context matches the expected size.
*/
uint32_t drmGetDrmContextSize( void );

/** @brief @b drmGetStrdSize returns number of bytes of strd chunk in the AVI this version of API can handle.

@return uint32_t
@retval Number of bytes of the strd.

@b Note: This should be same as DRM_STRD_SIZE_BYTES.  If the actual size of the strd chunk is less than this
returned value, the calling application should error out.  It is ok if the strd is larger because it may represent
a backward compatible chunk of larger size.
*/
uint32_t drmGetStrdSize( void );

/* Random number functions. */
/** @brief @b drmSetRandomSample creates the set of random samples used within the DRM routines.

@return drmErrorCodes_t
@retval DRM_SUCCESS - success.
@retval DRM_NOT_AUTHORIZED - failure.

@b Note: Call this at least 3 times from user input before call to drmGetRegistrationCode or drmInitPlayback.
DrmAdpLocal.c should be modified with the platform specific version of the function ::localClock().
*/
drmErrorCodes_t drmSetRandomSample( uint8_t* drmContext ); 

/** @brief @b drmGetRandomSampleCounter is used to determine the number of random numbers available.

@return uint32_t
@retval Number of random samples available.
*/ 
uint32_t drmGetRandomSampleCounter( uint8_t* drmContext ); /* Can be useful for making sure you have enough random samples. */

/* 
   Owner Registration Function. 
*/
/** @brief @b drmGetRegistrationCodeString is used to obtain the product registration code in an encrypted, human
readable form.

@param[out] registrationCodeString - Registration code

@return drmErrorCodes_t
@retval DRM_SUCCESS - success.
@retval DRM_NOT_AUTHORIZED - failure.

@b Note: DrmAdpLocal.c should be modified with the platform specific version of the function ::localLoadDrmMemory.
*/
drmErrorCodes_t drmGetRegistrationCodeString( uint8_t* drmContext,
                                              char *registrationCodeString );

/* 
   Owner Registration Function. 
*/
/** @brief @b drmGetDeactivationCodeString is used to obtain the product registration code in an encrypted, human
readable form.

@param[out] deactivationCodeString - Null terminated string representing the registration code.

@return drmErrorCodes_t
@retval DRM_SUCCESS - success.
@retval DRM_NOT_AUTHORIZED - failure.

@b Note: DrmAdpLocal.c should be modified with the platform specific version of the function ::localLoadDrmMemory.
*/
drmErrorCodes_t drmGetDeactivationCodeString( uint8_t* drmContext,
                                              char *deactivationCodeString );

/* 
   Owner Registration Function. 
*/
/** @brief @b drmGetActivationMessage is used to obtain the product registration code in an encrypted, human
readable form.

@param[out] activationMessage - Null terminated string representing the registration code.
@param[out] activationMessageLength - Null terminated string representing the registration code.

@return drmErrorCodes_t
@retval DRM_SUCCESS - success.
@retval DRM_NOT_AUTHORIZED - failure.

@b Note: DrmAdpLocal.c should be modified with the platform specific version of the function ::localLoadDrmMemory.
*/
drmErrorCodes_t drmGetActivationMessage( uint8_t* drmContext,
                                         int8_t* activationMessage,
                                         uint32_t* activationMessageLength );

/* 
   Owner Registration Function. 
*/
/** @brief @b drmGetDeactivationMessage is used to obtain the product registration code in an encrypted, human
readable form.

@param[out] messageString - Null terminated string representing the registration code.
@param[out] messageStringLength - Null terminated string representing the registration code.

@return drmErrorCodes_t
@retval DRM_SUCCESS - success.
@retval DRM_NOT_AUTHORIZED - failure.

*/
drmErrorCodes_t drmGetDeactivationMessage( uint8_t* drmContext,
                                           int8_t* messageString,
                                           uint32_t* messageStringLength  );

/* 
   Queries activation status.
*/
/** @brief @b drmGetActivationStatus is used to query the activation status so callers can determine if and who the
device is registered to.

@param[out] userId - User Id if this system has been properly activated.
@param[out] userIdLength - User Id length of the buffer being provided. Should be > 5 bytes otherwise an error will be returned.
This parameter will be modified to the actual bytes returned.

@return drmErrorCodes_t
@retval DRM_SUCCESS - success.
@retval DRM_NOT_AUTHORIZED - failure.

*/
drmErrorCodes_t drmGetActivationStatus( uint8_t* userId,
                                        uint32_t* userIdLength );

/* 
   Queries activation status.
*/
/** @brief @b drmIsDeviceActivated is used to query the activation status, will return whether or not the device is currently activated.

@return int8_t
@retval DRM_DEVICE_IS_ACTIVATED  - the device is currently activated with a valid user.
@retval DRM_DEVICE_NOT_ACTIVATED - the device is currnetly NOT activated with a valid user.

*/
int8_t drmIsDeviceActivated( void );

/** @brief @b drmInitSystem 

First call should have drmContext set to NULL.  The function will return the length of the drmContext required to be allocated
or passed into the function.

The second call will then subsequently take in the allocated drmContext and initialize it before playback.

This function must be called before any API functions are called and must be called for each playback.

@param[in] drmContext - Pointer to the buffer containing the memory for the drmContext
@param[in] drmContextLength - Pointer to length of the drmContext memory

@retval DRM_SUCCESS - success.
@retval DRM_NOT_AUTHORIZED - failure.


*/
drmErrorCodes_t drmInitSystem( uint8_t *drmContext,
                               uint32_t *drmContextLength );

/* 
   Playback functions. 
*/

/** @brief @b drmInitPlaybackContext 

@param[in] strdData - The strd data in the AVI file to be played.
@param[out] drmContext - Pointer to block of memory used for calls to ::drmQueryRentalStatus, ::drmCommitPlayback,
and ::drmDecryptVideo 

@retval DRM_SUCCESS - success.
@retval DRM_NOT_AUTHORIZED - failure.
*/
drmErrorCodes_t drmInitPlayback( uint8_t *drmContext,
                                 uint8_t *strdData );


/** @brief @b drmQueryRentalStatus is used to determine the rental status of the AVI file used to create the @c drmContext.

@param[in] drmContext - Pointer to the block of memory returned by ::drmInitPlaybackContext.
@param[out] rentalMessageFlag - Returned TRUE or FALSE if the rental number of views left screen needs to be displayed.
@param[out] useLimit - Returned the number of playbacks allowed for the file.
@param[out] useCount - Returned the number of time the file has been played back.

@return drmErrorCodes_t
@retval DRM_SUCCESS - success, check rentalFlag, useLimit, and useCount.
@retval DRM_RENTAL_EXPIRED - failure, rental expired.
@retval DRM_NOT_AUTHORIZED - failure.
*/
drmErrorCodes_t drmQueryRentalStatus( uint8_t *drmContext,
                                      uint8_t *rentalMessageFlag,
                                      uint8_t *useLimit,
                                      uint8_t *useCount );

/** @brief @b drmCommitPlayback 

@param[in] drmContext - Pointer to the block of memory returned by ::drmInitPlaybackContext.

@return drmErrorCodes_t
@retval DRM_SUCCESS - success.
@retval DRM_NOT_AUTHORIZED - failure.

*/
drmErrorCodes_t drmCommitPlayback( uint8_t *drmContext );


/** @brief @b drmFinalizePlayback

@param[in] drmContext - Pointer to the block of memory returned by ::drmInitPlaybackContext.

@return drmErrorCodes_t
@retval DRM_SUCCESS - success.
@retval DRM_NOT_AUTHORIZED - failure.

*/
drmErrorCodes_t drmFinalizePlayback( uint8_t *drmContext );

/** @brief @b drmDecryptVideo 

@param[in] drmContext - Pointer to the block of memory returned by ::drmInitPlaybackContext.
@param[in,out] frame - Pointer to memory buffer containing the encrypted frame data.
@param[in] frameSize - Size of the data in the @c frame buffer.
@param[in] drmFrameInfo - Pointer to the memory buffer containing the DRM info for the frame .

@return drmErrorCodes_t
@retval DRM_SUCCESS - success.
@retval DRM_NOT_AUTHORIZED - failure.
*/
drmErrorCodes_t drmDecryptVideo( uint8_t *drmContext,
                                 uint8_t *frame,
                                 uint32_t frameSize, 
                                 uint8_t *drmFrameInfo );

/** @brief @b drmDecryptAudio 

@param[in] drmContext - Pointer to the block of memory returned by ::drmInitPlaybackContext.
@param[in,out] frame - Pointer to memory buffer containing the encrypted frame data.
@param[in] frameSize - Size of the data in the @c frame buffer.

@return drmErrorCodes_t
@retval DRM_SUCCESS - success.
@retval DRM_NOT_AUTHORIZED - failure.
*/
drmErrorCodes_t drmDecryptAudio( uint8_t *drmContext,
                                 uint8_t *frame,
                                 uint32_t frameSize );

/** @brief @b drmQueryCgmsa - Query Copy Generation Management System - Analog

@param[in] drmContext - Pointer to the block of memory returned by ::drmInitPlaybackContext.
@param[out] cgmsaSignal - 0, 1, 2, or 3 based on standard CGMSA signaling.

@retval DRM_SUCCESS - success.
@retval DRM_NOT_AUTHORIZED - failure.
*/
drmErrorCodes_t drmQueryCgmsa( uint8_t *drmContext,
                               uint8_t *cgmsaSignal );

/** @brief @b drmQueryAcptb - Query Analog Copy Protection Trigger Bits

@param[in] drmContext - Pointer to the block of memory returned by ::drmInitPlaybackContext.
@param[out] acptbSignal - 0, 1, 2, or 3 based on standard trigger bit signaling.
acptb values:
0 = Off.
1 = Auto gain control / pseudo sync pulse.
2 = Two line color burst.
3 = Four line color burst.

@retval DRM_SUCCESS - success.
@retval DRM_NOT_AUTHORIZED - failure.
*/
drmErrorCodes_t drmQueryAcptb( uint8_t *drmContext,
                              uint8_t *acptbSignal );

/** @brief @b drmQueryDigitalProtection - Query if digital output protection should be turned on.

@param[in] drmContext - pointer to the block of memory returned by ::drmInitPlaybackContext
@param[out] digitalProtectionSignal - 0=off, 1=on.

@retval DRM_SUCCESS - success.
@retval DRM_NOT_AUTHORIZED - failure.
*/
drmErrorCodes_t drmQueryDigitalProtection( uint8_t *drmContext,
                                           uint8_t *digitalProtectionSignal );

/** @brief @b drmAdpGetLastError - Useful for debugging and getting more detailed error messages.

@retval error code defined in DrmAdpApi.c (used for debugging).
*/
drmErrorCodes_t drmGetLastError( uint8_t* drmContext );

#if 0
/*!
 Function prototype for the API function called at any time to return
 the version of the implementing library.
  
  @note the char * members of the struct do not need
        memory allocated; the pointed to location will be updated.

  @param pVersion (IN/OUT) Pointer to a library structure into which values will be copied.
  @return DIVXVERSION_OK for success, otherwise returns a DIVXVERSION_ error code.

 */
int drmGetVersion( DivXVersion *pVersion );
#endif

#if 1
drmErrorCodes_t s_saveDrmMemory( drmMemory_t *memory );

drmErrorCodes_t drmQueryFrameKeys(uint8_t* drmContext, uint8_t* frame_keys_count, uint8_t* frame_keys);

drmErrorCodes_t drmQueryAudioProtection(uint8_t* drmContext, audioCryptoInfo_t* audioCryptoInfo);

drmErrorCodes_t drmIncreaseRental(playbackContext_t *drmContext);

#endif

/**  @}  */

#ifdef __cplusplus
};
#endif

#endif
/* DRMAPI_H_INCLUDE */
