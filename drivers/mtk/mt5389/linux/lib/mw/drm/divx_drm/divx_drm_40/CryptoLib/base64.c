/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_40/CryptoLib/base64.c#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef __KERNEL__

#include "base64.h"

#ifdef _MTK_BUILD_
#include "libc/string.h"
#include "libc/stdlib.h"
#include "libc/memory.h"
#else
#include <string.h>
#include <stdlib.h>
#endif

#else

#include "base64.h"
#include "../../divx_drm_adp_common.h" /* NULL */

#include "x_os.h" /* malloc/free */
#define malloc      x_mem_alloc
#define free        x_mem_free

#endif

static
char _base64Chars[]= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
				     'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
			         '0','1','2','3','4','5','6','7','8','9',
			         '-','_' };


int base64CharValue(char c)
{
    int i;

    for(i=0; i < 64; i++)
    {
        if(_base64Chars[i] == c)
        {
            return i;
        }
    }

    return -1;
}

void base64EncodeBlock(char *block, char *dst, unsigned int size)
{
	dst[0] = '=';
	dst[1] = '=';
	dst[2] = '=';
	dst[3] = '=';

    switch(size)
    {
    case 3:
        {
            int b1 = (int)((block[0] & 0xFC) >> 2);
            int b2 = (int)(((block[0] & 0x03) << 4) | ((block[1] & 0xF0) >> 4));
            int b3 = (int)(((block[1] & 0x0F) << 2) | ((block[2] & 0xC0) >> 6));
            int b4 = (int)((block[2] & 0x3F));

            dst[0] = _base64Chars[b1]; 
            dst[1] = _base64Chars[b2]; 
            dst[2] = _base64Chars[b3]; 
            dst[3] = _base64Chars[b4]; 

        }
        break;
    
    case 2:
        {
            int b1 = (int)((block[0] & 0xFC) >> 2);
            int b2 = (int)(((block[0] & 0x03) << 4) | ((block[1] & 0xF0) >> 4));
            int b3 = (int)((block[1] & 0x0F) << 2);

            dst[0] = _base64Chars[b1]; 
            dst[1] = _base64Chars[b2]; 
            dst[2] = _base64Chars[b3]; 
            
        }
        break;
    
    case 1:
        {
            int b1 = (int)((block[0] & 0xFC) >> 2);
            int b2 = (int)((block[0] & 0x03) << 4);
            
            dst[0] = _base64Chars[b1]; 
            dst[1] = _base64Chars[b2]; 
        }
        break;
	/* Should not hit this ever... but this will essentially do nothing. */
	default:
		break;
    }
}


void base64DecodeBlock(char *block, char *out)
{
    if((block[3] == '=') && (block[2] == '='))
    {
		int v1;
        int c1 = base64CharValue(block[0]);
        int c2 = base64CharValue(block[1]);

        if((c1 == -1) || (c2 == -1))
        {
            return;
        }

        v1 = (c1 << 2) | (c2 >> 4); 
        
		out[0] = (char) v1;
        out[1] = '\0';
    }
    else
    {
        if(block[3] == '=')
        {
			int v1, v2;
            int c1 = base64CharValue(block[0]);
            int c2 = base64CharValue(block[1]);
            int c3 = base64CharValue(block[2]);

            if((c1 == -1) || 
               (c2 == -1) ||
               (c3 == -1))
            {
                return;
            }

            v1 = (c1 << 2) | ((c2 & 0x30) >> 4); 
            v2 = ((c2 & 0x0F)<< 4) | ((c3 & 0x3C) >> 2); 
            
            out[0] = (char) v1;
            out[1] = (char) v2;
            out[2] = '\0';
        }
        else
        {
			int v1, v2, v3;
            int c1 = base64CharValue(block[0]);
            int c2 = base64CharValue(block[1]);
            int c3 = base64CharValue(block[2]);
            int c4 = base64CharValue(block[3]);

            if((c1 == -1) || 
               (c2 == -1) ||
               (c3 == -1) ||
               (c4 == -1))
            {
                return;
            }

            v1 = (c1 << 2) | ((c2 & 0x30) >> 4); 
            v2 = ((c2 & 0x0F)<< 4) | ((c3 & 0x3C) >> 2); 
            v3 = ((c3 & 0x03)<< 6) | ((c4 & 0x3F)); 

            out[0] = (char) v1;
            out[1] = (char) v2;
            out[2] = (char) v3;
        }
    }
}

char *base64Encode(char *buffer, unsigned int length, unsigned int *outlength)
{
	char* sourcePointer = NULL;
	char* resultPointer = NULL;
	unsigned int i;
    unsigned int blocks = length / 3;
    unsigned int left   = length % 3;
	unsigned int size   = blocks*4 + (left != 0 ? 4 : 0);

	char *result = (char *) malloc(size * sizeof(char));
	if(result != NULL)
	{
		memset(result, 0, size * sizeof(char));

		for(i=0; i < blocks; i++)
		{
			sourcePointer = buffer + (i * 3);
			resultPointer = result + (i * 4);
	
			base64EncodeBlock(sourcePointer, resultPointer, 3);
		}

		if(left > 0)
		{
			sourcePointer = buffer + (blocks)*3;
			resultPointer = result + (blocks)*4;
			base64EncodeBlock(sourcePointer, resultPointer, left);
		}

		*outlength = size;
	}
	else
	{
		*outlength = 0;
	}

	return result;
}

char *base64Decode(char *str,
				   unsigned int length,
				   unsigned int *outlength)
{
	char *result = NULL;
	unsigned int i;
    unsigned int blocks   = length/4;
    unsigned int outSize  = blocks*3;
    unsigned int realSize = outSize;

    if(((length % 4) > 0) || ((length == 0)   ))
    {
        return result;
    }

	if(1 == validBase64Data(str, length))
	{
	    if(str[length - 1] == '=')
		{
			realSize--;

	        if(str[length - 2] == '=')
		    {
			    realSize--;
	        }
		}

		result = (char *) malloc(outSize * sizeof(char));
		if(result == NULL)
		{
			*outlength = 0;
			return NULL;
		}

		for(i=0; i < blocks; i++)
		{
	        base64DecodeBlock(str + 4*i, result + i*3);
	    }

		*outlength = realSize;
	}
	else
	{
		*outlength = 0;
	}

	return result;
}

int validBase64Data(char *str, unsigned int length)
{
	int result = 1;
	unsigned int index = 0;
	int base64Index = 0;
	char currentCharacter;
	int found = 0;

	for(index = 0; index < length; index++)
	{
		currentCharacter = str[index];
		if(currentCharacter == '\0')
		{
			break;
		}

		found = 0;	
		for(base64Index = 0; base64Index < 64; base64Index++)
		{
			if(currentCharacter == _base64Chars[base64Index])
			{
				found = 1;
				break;
			}
		}

		if(0 == found)
		{
			if(currentCharacter != '=')
			{	result = 0;
				break;
			}
		}
	}

	return result;
}



static char base32[32] = {
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'M',
    'N',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z'
};

void base32Encode40Bits(const unsigned char *input, char *output)
{
	unsigned char current = '\0';
	unsigned char number = '\0';
	unsigned char bits[5] = {0};
	unsigned char i = (unsigned char)0;
	unsigned char j = (unsigned char)0;
	unsigned char k = (unsigned char)0;
	unsigned char m = (unsigned char)0;

	current = input[0];

	for (i = (unsigned char)1; i < (unsigned char)41; i = i + (unsigned char)1) /* Index from 1 to make code easier. */
	{
		/* Peel off one bit, and store in array. */
		bits[m++] = current & 0x80;
		current = current << 1;

		/* Check to get new input character. */
		if (0 == (i % 8))
		{
			j++;
			if (j < 5) /* Handle boundary case, do not copy in new value at end. */
			{
				current = input[j];
			}
		}

		/* Check to get new output character */
		if (0 == (i % 5))
		{
			number = ((bits[0] >> 0) |
				      (bits[1] >> 1) |
					  (bits[2] >> 2) |
					  (bits[3] >> 3) |
					  (bits[4] >> 4)) >> 3;
			output[k] = base32[number];
			memset(bits, 0, 5);
			k++;
			m = (unsigned char)0;
		}
	}

	output[8] = '\0';
}

void base32Decode40Bits(const char *input, unsigned char *output)
{
	unsigned char number = (unsigned char)0;
	unsigned char bits[40];
	unsigned char i = (unsigned char)0;
	int j = 0;
	unsigned char k = (unsigned char)0;
	unsigned char m = (unsigned char)0;

	memset(bits, 0, sizeof(char) * 40);

	if(1 == validBase32Data(input, strlen(input)))
	{
		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 32; j++)
			{
				if (base32[j] == input[i])
				{
					number = j;
					number = number << 3;
					for (k = 0; k < 5; k++)
					{
						if ( (number & 0x80) == 0x80)
						{
							bits[m] = 0x01;
						}
						m++;
						number = number << 1;
					}
					break;
				}
			}
		}	
	}
	else
	{
		return;
	}

	m = 0;
	number = 0;
	for (i = 0; i < 5; i++)
	{
		for (j = 7; j >= 0; j--)
		{
			number = number | (bits[m++] << j);
		}
		output[i] = number;
		number = 0;
	}
}

int validBase32Data(const char *str, unsigned int length)
{
	int result = 1;
	unsigned int index = 0;
	int base32Index = 0;
	char currentCharacter;
	int found = 0;

	for(index = 0; index < length; index++)
	{
		currentCharacter = str[index];
		if(currentCharacter == 0)
		{	result = 0;
			break;
		}

		found = 0;	
		for(base32Index = 0; base32Index < 32; base32Index++)
		{
			if(currentCharacter == base32[base32Index])
			{
				found = 1;
				break;
			}
		}

		if(found == 0)
		{
			result = 0;
			break;
		}
	}

	return result;
}
