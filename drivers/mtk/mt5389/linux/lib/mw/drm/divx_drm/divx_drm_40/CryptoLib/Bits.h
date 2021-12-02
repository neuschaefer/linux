/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_40/CryptoLib/Bits.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

/*
These functions help with bit manipulation and conversions of data.
*/

#ifndef BITS_H_INCLUDE
#define BITS_H_INCLUDE

#ifndef __KERNEL__

#include "divx_drm_adp_common.h"

#else

#include "../../divx_drm_adp_common.h"

#endif

#define A_BECOMES_B					1
#define B_BECOMES_A					2
#define HEX_OCTET_PER_BYTE          2

#ifdef __cplusplus
extern "C" {
#endif

uint16_t pack(uint8_t low, uint8_t hi);
void unpack(uint16_t whole, uint8_t *hi, uint8_t *low);
void assign(uint8_t *a, uint8_t *b, uint8_t direction);

void quarterByteSwap(uint8_t *a, uint8_t *b, uint8_t position);
void halfByteSwap(uint8_t *a, uint8_t *b, uint8_t position);
void maskSwap(uint8_t *a, uint8_t *b, uint8_t mask);

uint8_t getByteLoopSizeFromBits(uint8_t sizeInBits);
void convertBinToHex(const uint8_t *binary, uint32_t binarySizeBytes, char *allocatedHexString);
int32_t convertHexToBin(const char *hexString, uint32_t allocatedBinaryDataSize, uint8_t *allocatedBinaryData);

#ifdef __cplusplus
};
#endif

#endif
/* BITS_H_INCLUDE */
