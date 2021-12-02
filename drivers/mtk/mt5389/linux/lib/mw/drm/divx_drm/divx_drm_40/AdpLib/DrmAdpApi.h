/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_40/AdpLib/DrmAdpApi.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

/*
These functions are the top level interface to the Authorized Disconnected Playback (ADP)
Digital Rights Management (DRM) capabilities.  The names correspond to the  DivX
Authorized Disconnected Playback document and the user interface flow.
*/

/** @defgroup DRM adpLib API
 *  Description of Digital Rights Management (DRM) Authorized Disconnected Playback (ADP)
 *  @{
 */

#ifndef DRMADPAPI_H_INCLUDE
#define DRMADPAPI_H_INCLUDE

#ifndef __KERNEL__

#include "divx_drm_adp_common.h"
#include "AdpLib/DrmAdpTypes.h"
#include "AdpKeyStoreLib/DrmAdpKeyStore.h"

#else

#include "../../divx_drm_adp_common.h"
#include "DrmAdpTypes.h"
#include "../AdpKeyStoreLib/DrmAdpKeyStore.h"

#endif


/* Sizes. */
/* #define DRM_CONTEXT_SIZE_BYTES			2488 //2432 //sizeof(PlaybackContext) */
#define DRM_REGISTRATION_CODE_BYTES		9
#define DRM_STRD_SIZE_BYTES				2224
#define DRM_FRAME_DRM_INFO_SIZE			10

/* Return values. */
#define ADP_SUCCESS                     0
#define ADP_NOT_AUTHORIZED              1
#define ADP_RENTAL_EXPIRED              2

typedef struct PlaybackContextStruct
{
	uint8_t baseKeyId[ADP_BASE_KEY_ID_SIZE_BYTES];
	DrmAdpTargetHeader adpTarget;
	DrmHeaderSignature signature;
	VideoFrameKeys frameKeys;
	uint8_t state;
       DivXBool b_save_drm_memory;
       DrmMemory t_drm_memory;
} PlaybackContext;

#ifdef __cplusplus
extern "C" {
#endif

/** @brief @b getPlaybackContextSize returns number of bytes of context.

@return uint32_t
@retval Number of bytes of the context.

@b Note: This should be same as DRM_CONTEXT_SIZE_BYTES.  This function can provide a check during
development that the implementation of the context matches the expected size.
*/
uint32_t getPlaybackContextSize(void);

/** @brief @b getStrdDataSize returns number of bytes of strd chunk in the AVI this version of API can handle.

@return uint32_t
@retval Number of bytes of the strd.

@b Note: This should be same as DRM_STRD_SIZE_BYTES.  If the actual size of the strd chunk is less than this
returned value, the calling application should error out.  It is ok if the strd is larger because it may represent
a backward compatible chunk of larger size.
*/
uint32_t getStrdDataSize(void);

/* Random number functions. */
/** @brief @b drmSetRandomSample creates the set of random samples used within the DRM routines.

@return void 

@b Note: Call this at least 3 times from user input before call to drmGetRegistrationCode or drmInitPlayback.
DrmAdpLocal.c should be modified with the platform specific version of the function ::localClock().
*/
void drmSetRandomSample(void); 

/** @brief @b drmGetRandomSampleCounter is used to determine the number of random numbers available.

@return uint32_t
@retval Number of random samples available.
*/ 
uint32_t drmGetRandomSampleCounter(void); /* Can be useful for making sure you have enough random samples. */

/* 
   Owner Registration Function. 
*/
/** @brief @b drmGetRegistrationCodeString is used to obtain the product registration code in an encrypted, human
readable form.

@param[out] registrationCodeString - Null terminated string representing the registration code.

@return uint8_t
@retval ADP_SUCCESS - success.
@retval ADP_NOT_AUTHORIZED - failure.

@b Note: DrmAdpLocal.c should be modified with the platform specific version of the function ::localLoadDrmMemory.
*/
uint8_t drmGetRegistrationCodeString(char *registrationCodeString);

/* 
   Playback functions. 
*/
/** @brief @b drmInitPlaybackContext 

@param[in] strdData - The strd data in the AVI file to be played.
@param[out] drmContext - Pointer to block of memory used for calls to ::drmQueryRentalStatus, ::drmCommitPlayback,
and ::drmDecryptVideo 

@retval ADP_SUCCESS - success.
@retval ADP_NOT_AUTHORIZED - failure.
*/
uint8_t drmInitPlaybackContext(uint8_t *strdData, uint8_t *drmContext);


/** @brief @b drmQueryRentalStatus is used to determine the rental status of the AVI file used to create the @c drmContext.

@param[in] drmContext - Pointer to the block of memory returned by ::drmInitPlaybackContext.
@param[out] rentalMessageFlag - Returned TRUE or FALSE if the rental number of views left screen needs to be displayed.
@param[out] useLimit - Returned the number of playbacks allowed for the file.
@param[out] useCount - Returned the number of time the file has been played back.

@return uint8_t
@retval ADP_SUCCESS - success, check rentalFlag, useLimit, and useCount.
@retval ADP_RENTAL_EXPIRED - failure, rental expired.
@retval ADP_NOT_AUTHORIZED - failure.
*/
uint8_t drmQueryRentalStatus(uint8_t *drmContext,
							 uint8_t *rentalMessageFlag,
							 uint8_t *useLimit,
							 uint8_t *useCount);

/** @brief @b drmCommitPlayback 

@param[in] drmContext - Pointer to the block of memory returned by ::drmInitPlaybackContext.

@return uint8_t
@retval ADP_SUCCESS - success.
@retval ADP_NOT_AUTHORIZED - failure.

*/
uint8_t drmCommitPlayback(uint8_t *drmContext);

/** @brief @b drmQueryCgmsa - Query Copy Generation Management System - Analog

@param[in] drmContext - Pointer to the block of memory returned by ::drmInitPlaybackContext.
@param[out] cgmsaSignal - 0, 1, 2, or 3 based on standard CGMSA signaling.

@retval ADP_SUCCESS - success.
@retval ADP_NOT_AUTHORIZED - failure.
*/
uint8_t drmQueryCgmsa(uint8_t *drmContext, uint8_t *cgmsaSignal);

/** @brief @b drmQueryAcptb - Query Analog Copy Protection Trigger Bits

@param[in] drmContext - Pointer to the block of memory returned by ::drmInitPlaybackContext.
@param[out] acptbSignal - 0, 1, 2, or 3 based on standard trigger bit signaling.
acptb values:
0 = Off.
1 = Auto gain control / pseudo sync pulse.
2 = Two line color burst.
3 = Four line color burst.

@retval ADP_SUCCESS - success.
@retval ADP_NOT_AUTHORIZED - failure.
*/
uint8_t drmQueryAcptb(uint8_t *drmContext, uint8_t *acptbSignal);

/** @brief @b drmQueryDigitalProtection - Query if digital output protection should be turned on.

@param[in] drmContext - pointer to the block of memory returned by ::drmInitPlaybackContext
@param[out] digitalProtectionSignal - 0=off, 1=on.

@retval ADP_SUCCESS - success.
@retval ADP_NOT_AUTHORIZED - failure.
*/
uint8_t drmQueryDigitalProtection(uint8_t *drmContext, uint8_t *digitalProtectionSignal);

/** @brief @b drmQueryFrameKeys - Query Frame Key Table.
*/
uint8_t drmQueryFrameKeys(uint8_t* drmContext, uint8_t* frame_keys_count, uint8_t* frame_keys);

/** @brief @b drmQueryAudioProtection - Query Audio Protection Information.
*/
uint8_t drmQueryAudioProtection(uint8_t* drmContext, AudioCryptoInfo* audioCryptoInfo);

/** @brief @b drmAdpGetLastError - Useful for debugging and getting more detailed error messages.

@retval error code defined in DrmAdpApi.c (used for debugging).
*/
uint8_t drmAdpGetLastError(void);

uint8_t drmIncreaseRental(PlaybackContext *context);

uint8_t saveDrmMemory(DrmMemory *memory);

/**  @}  */

#ifdef __cplusplus
};
#endif

#endif
/* DRMADPAPI_H_INCLUDE */
