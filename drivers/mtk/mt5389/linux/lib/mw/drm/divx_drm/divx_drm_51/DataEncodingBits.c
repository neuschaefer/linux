/*
   // $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/DataEncodingBits.c#1 $
   // Copyright (c) 2006 DivX, Inc. http://www.divx.com/corporate
   // All rights reserved.
   //
   // This software is the confidential and proprietary information of DivX
   // Inc. ("Confidential Information").  You shall not disclose such Confidential
   // Information and shall use it only in accordance with the terms of the license
   // agreement you entered into with DivX, Inc.
 */
#ifndef __KERNEL__

#include "DataEncodingBits.h"

#ifdef _MTK_BUILD_
#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif
#include "libc/stdio.h"
#include "libc/string.h"
#else
#include <limits.h> // CHAR_BIT
#include <stdio.h>  // sprintf
#include <string.h> // strlen
#endif

#else

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

#include "DataEncodingBits.h"

#endif

#define HEX_OCTET_PER_BYTE          (2)

void encodingAssign(uint8_t *a, uint8_t *b, uint8_t direction)
{
    if (direction == ENCODING_DIRECTION_A_BECOMES_B)
    {
        a[0] = b[0];
    }
    if (direction == ENCODING_DIRECTION_B_BECOMES_A)
    {
        b[0] = a[0];
    }
}

void encodingQuarterByteSwap(uint8_t *a, uint8_t *b, uint8_t position)
{
	
	uint8_t mask;

	switch (position)
	{
	case 0:
		mask = 0x3F;
		break;
	case 1:
		mask = 0xCF;
		break;
	case 2:
		mask = 0xF3;
		break;
	case 3:
		mask = 0xFC;
		break;
	default:
		mask = 0x3F;

	}

	encodingMaskSwap(a, b, mask);
}

void encodingHalfByteSwap(uint8_t *a, uint8_t *b, uint8_t position)
{
	
	uint8_t mask;

	switch (position)
	{
	case 0:
		mask = 0x0F;
		break;
	case 1:
		mask = 0xF0;
		break;
	default:
		mask = 0x0F;

	}

	encodingMaskSwap(a, b, mask);
}

void encodingMaskSwap(uint8_t *a, uint8_t *b, uint8_t mask)
{
	uint8_t aResult;
	uint8_t bResult;

	aResult = (*a & mask) | (*b & ~mask);
	bResult = (*b & mask) | (*a & ~mask);

    *a = aResult;
	*b = bResult;
}

uint16_t encodingPack(uint8_t low, uint8_t hi)
{
    uint16_t result = hi;
    result = (result << 8) | low;
    return result;
}

void encodingUnpack(uint16_t whole, uint8_t *hi, uint8_t *low)
{
    *hi = (uint8_t)((whole & 0xff00) >> 8);
    *low = (uint8_t)(whole & 0xff);
}

uint8_t encodingGetByteLoopSizeFromBits(uint8_t sizeInBits)
{
    uint8_t result;
    
    result = sizeInBits / CHAR_BIT;
    if ((sizeInBits % CHAR_BIT) > 0)
    {
        result++;
    }
    
    return result;
}

void encodingConvertBinToHex(const uint8_t *binary, uint32_t binarySizeBytes, char *allocatedHexString)
{
	uint32_t index;

	for (index = 0; index < binarySizeBytes; index++)
	{
		sprintf(&allocatedHexString[HEX_OCTET_PER_BYTE * index], "%02X", binary[index]);
	}
}

int32_t encodingConvertHexToBin(const char *hexString, uint32_t allocatedBinaryDataSize, uint8_t *allocatedBinaryData)
{
	uint32_t i;
	uint32_t j;
	uint8_t t;
	uint8_t v;

	j = 0;
	for (i = 0; i < strlen(hexString) - 1; i = i + 2)
	{
		t = hexString[i];

		if ((t >= '0') && (t <= '9'))
		{
			v = (t - '0') << 4;
		}
		else
		{
			if ((t >= 'a') && (t <= 'f'))
			{
				v = (t - 'a' + 10) << 4;
			}
			else
			{
				if ((t >= 'A') && (t <= 'F'))
				{
					v = (t - 'A' + 10) << 4;
				}
				else
				{
					return 1;  // Error, not hex data.
				}
			}
		}

		t = hexString[i + 1];
		if ((t >= '0') && (t <= '9'))
		{
			v ^= (t - '0');
		}
		else
		{
			if ((t >= 'a') && (t <= 'f'))
			{
				v ^= (t - 'a' + 10);
			}
			else
			{
				if ((t >= 'A') && (t <= 'F'))
				{
					v ^= (t - 'A' + 10);
				}
				else
				{
					return 1;  // Error, not hex data.
				}
			}
		}

		allocatedBinaryData[j] = v;
		j++;

		if (j > allocatedBinaryDataSize)
		{
			return 1; // Error, not enough space to put the data.
		}
	}

	return 0; // Success.
}
