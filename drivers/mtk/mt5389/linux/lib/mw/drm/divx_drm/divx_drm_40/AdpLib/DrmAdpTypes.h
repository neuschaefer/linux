/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_40/AdpLib/DrmAdpTypes.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef DRMADPTYPES_H_INCLUDE
#define DRMADPTYPES_H_INCLUDE

#ifndef __KERNEL__

#include "divx_drm_adp_common.h"

#else

#include "../../divx_drm_adp_common.h"

#endif

#define VIDEO_KEY_SIZE_BYTES				16
#define VIDEO_CRYPTO_BLOCK_SIZE_BYTES		16
#define VIDEO_KEY_SIZE_BITS					128
#define VIDEO_KEY_COUNT_MAX					128
#define VIDEO_MAX_ENCRYPT_BYTES             512
#define VIDEO_FRAME_PROTECTION_INFO_BYTES    10

#define KEY_INDEX_SIZE 2
#define OFFSET_SIZE    4
#define CRYPTO_SIZE    4

typedef struct VideoFrameKeysStruct
{
	uint8_t keys[VIDEO_KEY_COUNT_MAX][VIDEO_KEY_SIZE_BYTES];
} VideoFrameKeys;

typedef struct VideoCryptoInfoStruct
{
        uint8_t count;
        VideoFrameKeys frameKeys;
} VideoCryptoInfo;

#define AUDIO_USE_VIDEO_KEY_INDEX           0
#define AUDIO_KEY_SIZE_BYTES				16

typedef struct AudioCryptoInfoStruct
{
#define PROTECTED_AUDIO 1
        uint8_t flag;
        uint8_t key[AUDIO_KEY_SIZE_BYTES];
        uint8_t offset;
        uint8_t size;
} AudioCryptoInfo;

/* Sizes */
#define OWNER_GUARD_BYTES				3
#define OWNER_USER_ID_BYTES				5
#define SLOT_USE_DATA_BYTES				1
#define SLOT_SERIAL_NUMBER_BYTES		2
#define USE_LIMITS						8
#define TRANSACTION_ID_BYTES			46
#define MODEL_ID_BYTES					2   /* Model id only uses 12 of the bits. */
#define KEY_SIZE_BYTES					16
#define KEY_SIZE_BITS					128
#define DRM_BASE_KEY_ID_LENGTH			44
#define DRM_ADP_RESERVED				2
#define DRM_OTHER_RESERVED				12
#define DRM_ADP_RENTAL_RESERVED_BYTES	3
#define CGMSA_BITS						2
#define ACPTB_BITS						2
#define DIGITAL_PROTECTION_BITS			1
#define OUTPUT_SIGNAL_RESERVED_BITS		3
#define SHA256_SIZE_BYTES				32
#define ADP_RSA_KEY_SIZE_BYTES			256 /* If you change this size, you need to change the number of frame keys in VideoDecryptTypes */
#define ADP_RSA_KEY_SIZE_BITS			2048 /* = RSA_KEY_BYTES * CHAR_BIT */
#define ADP_RSA_HEX_KEY_SIZE_BYTES		((ADP_RSA_KEY_SIZE_BYTES * 2) + 1)
#define ADP_RSA_HEX_E_EXP_SIZE_BYTES	((ADP_RSA_KEY_SIZE_BYTES * 2 / 32) + 1)
#define ADP_DIGEST_RESERVED_BYTES		4
#define ADP_SIGNATURE_RESERVED_BYTES	12
#define ADP_SIGNATURE_CRYPTO_BLOCKS		1
#define ADP_SIGNATURE_SUB_CRYPTO_BYTES	8
#define MAX_MESSAGE_SIZE_BYTES			12
#define VIDEO_KEY_COUNT_FOR_SIGNED		111

/* DRM Modes */
#define DRM_TYPE_BASE					0xF0F0
#define DRM_TYPE_ACTIVATION_PURCHASE	0xC3C3
#define DRM_TYPE_ACTIVATION_RENTAL		0x3C3C
#define DRM_TYPE_PURCHASE				0x5555
#define DRM_TYPE_RENTAL					0xAAAA
#define DRM_TYPE_PROTECTED_AUDIO		0x9797

/* Use Limits. */
#define USE_LIMIT_ID_UNLIMITED			0x0707
#define USE_LIMIT_ID_ONE				0x1001
#define USE_LIMIT_ID_THREE				0x3003
#define USE_LIMIT_ID_FIVE				0x5005
#define USE_LIMIT_ID_SEVEN				0x7007
#define USE_LIMIT_ID_TEN				0xAAAA
#define USE_LIMIT_ID_TWENTY				0x5555
#define USE_LIMIT_ID_THIRTY				0xF0F0

/* Option masks. */
#define OUTPUT_PROTECTION_MASK			0x01
#define SIGNED_ACTIVATION_MASK			0x02
#define PROTECTED_AUDIO_MASK			0x04
#define VARIABLE_FRAME_KEY_COUNT_MASK	0x08

#define TOTAL_PLAY_SLOTS		8
#define PACKED_ALLOCATION_BYTES	48

typedef struct DrmUseLimitIdInfoStruct
{
    uint16_t id;
    uint8_t uses;
} DrmUseLimitIdInfo;

/* Move to DrmAdpApi.c to eliminate warnings.
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
*/

typedef struct DrmActivateRecordStruct
{
	uint8_t memoryGuard[OWNER_GUARD_BYTES];
	uint8_t modelId[MODEL_ID_BYTES];
	uint8_t userKey[KEY_SIZE_BYTES];
	uint8_t explicitGuard[OWNER_GUARD_BYTES];
} DrmActivateRecord;

typedef struct DrmRentalRecordStruct
{
	uint16_t useLimitId;
	uint8_t serialNumber[SLOT_SERIAL_NUMBER_BYTES];
	uint8_t slotNumber;
	uint8_t reserved[DRM_ADP_RENTAL_RESERVED_BYTES];
} DrmRentalRecord;

typedef struct DrmAdpTargetHeaderStruct
{
	uint16_t drmMode;
	uint8_t userId[OWNER_USER_ID_BYTES];
	uint8_t optionFlags;
    DrmRentalRecord rentalRecord;
	uint8_t sessionKey[KEY_SIZE_BYTES];
	DrmActivateRecord activateRecord;
	uint8_t outputProtectionFlags;
	uint8_t protectedAudioOffset;
	uint8_t protectedAudioCryptoSize;
	uint8_t frameKeyCount;
	uint16_t drmSubMode;
	uint8_t reserved[DRM_ADP_RESERVED];
} DrmAdpTargetHeader;

typedef struct DrmTransactionInfoHeaderStruct
{
	uint8_t transactionId[TRANSACTION_ID_BYTES];
	uint16_t transactionAuthorityId;
	uint32_t contentId;
	uint8_t reserved[DRM_OTHER_RESERVED];
} DrmTransactionInfoHeader;

typedef struct DrmHeaderSignatureStruct
{
	uint32_t signerId;
	uint8_t reserved[ADP_SIGNATURE_RESERVED_BYTES];
	uint8_t signedData[ADP_RSA_KEY_SIZE_BYTES];
} DrmHeaderSignature;

typedef struct DrmHeaderStruct
{
	uint32_t reservedFlags;
	uint8_t baseKeyId[DRM_BASE_KEY_ID_LENGTH];
	DrmAdpTargetHeader adpTarget;
	DrmTransactionInfoHeader transaction;
	uint8_t frameKeys[VIDEO_KEY_COUNT_FOR_SIGNED][VIDEO_KEY_SIZE_BYTES];
} DrmHeader;

typedef struct DrmDigestStruct
{
	uint8_t hash[SHA256_SIZE_BYTES];
	uint8_t key[KEY_SIZE_BYTES];
	uint8_t reserved[ADP_DIGEST_RESERVED_BYTES];
} DrmDigest;


typedef struct OwnerSlotStruct
{
    uint8_t guard[OWNER_GUARD_BYTES];
    uint8_t key[KEY_SIZE_BYTES];
    uint8_t userId[OWNER_USER_ID_BYTES];
} OwnerSlot;

typedef struct PlaySlotStruct
{
    uint8_t serialNumber[SLOT_SERIAL_NUMBER_BYTES];
    uint8_t counter;
} PlaySlot;

typedef struct DrmMemoryStruct
{
    OwnerSlot owner;
    PlaySlot slots[TOTAL_PLAY_SLOTS];
} DrmMemory;

typedef struct DrmPackedMemoryStruct
{
    uint8_t packed[PACKED_ALLOCATION_BYTES];
} DrmPackedMemory;

typedef struct DrmMessagePackedStruct
{
    uint8_t message[MAX_MESSAGE_SIZE_BYTES];
    uint8_t sizeInBits;
} DrmMessagePacked;

typedef struct RegistrationRequestStruct
{
    uint8_t userIdGuard[OWNER_GUARD_BYTES];
    uint8_t modelId[MODEL_ID_BYTES];
} RegistrationRequest;


#endif
/* DRMADPTYPES_H_INCLUDE */
