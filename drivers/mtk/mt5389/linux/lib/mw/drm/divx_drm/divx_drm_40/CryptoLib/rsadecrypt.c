/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_40/CryptoLib/rsadecrypt.c#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef __KERNEL__

#include "rsadecrypt.h"

#include "bn.h"
#include "Bits.h"

#ifdef _MTK_BUILD_
#include "libc/memory.h"
#include "libc/string.h"
#else
#include <string.h>
#endif

#else

#include "rsadecrypt.h"

#include "bn.h"
#include "Bits.h"

#endif

uint8_t rsaPublicDecrypt255(char nHex[RSA_SIZE_NHEX_STRING],
							char eHex[RSA_SIZE_EHEX_STRING],
							uint8_t ciphered[RSA_SIZE_CIPHERED_BYTES],
							uint8_t plain[RSA_SIZE_PLAIN_BYTES])
{
	/* Based of bn (big number) arithmitic. */

    BN_CTX *bnContext;
	BIGNUM *c;
	BIGNUM *t;
	BIGNUM *e;
	BIGNUM *n;
	char bitsHex[RSA_SIZE_NHEX_STRING];
	char paddedBitsHex[RSA_SIZE_NHEX_STRING];
	uint8_t result = 1;
	uint8_t fullPlain[RSA_SIZE_PLAIN_BYTES];
	int i;
	int padSize;

	/* Initialize. */
	bnContext = BN_CTX_new();
	c = BN_new();
	t = BN_new();
	e = BN_new();
	n = BN_new();

	/* Get ciphered into bn. */
	BN_bin2bn(ciphered, RSA_SIZE_CIPHERED_BYTES, c);

	/* Get key data. */
	BN_hex2bn(&n, nHex);
	BN_hex2bn(&e, eHex);

	/* Decrypt. */
    BN_mod_exp(t, c, e, n, bnContext);

	/* Handle the string with leading zeros, which keeps everything aligned. */
	strcpy(bitsHex, BN_bn2hex(t)); /* Reusing bitsHex. */
	if (RSA_SIZE_NHEX_STRING != strlen(bitsHex))
	{
		/* There are leading zeros. Add them. */
		padSize = RSA_SIZE_NHEX_STRING - strlen(bitsHex);
		padSize = padSize - 1; /* Do not count the null. */
		for (i = 0; i < RSA_SIZE_NHEX_STRING; i++)
		{
			if (i < padSize)
			{
				paddedBitsHex[i] = '0';
			}
			else
			{
				paddedBitsHex[i] = bitsHex[i - padSize];
			}
		}
	}

	/* Handle the leading zero and convention of encrypt. */
	if ( convertHexToBin(paddedBitsHex, RSA_SIZE_CIPHERED_BYTES, fullPlain) != 0 )
	{
		result = 1;
	}
	else
	{
		if ( 0 == fullPlain[0] )  /* Convention of encrypt. */
		{
			memcpy(plain, fullPlain + 1, RSA_SIZE_PLAIN_BYTES);  /* Skip first byte, it was zero. */
			result = 0;
		}
	}

	/* Clean up. */
	BN_free(c);
	BN_free(e);
	BN_free(n);
	BN_free(t);
	BN_CTX_free(bnContext);

	return result;
}
