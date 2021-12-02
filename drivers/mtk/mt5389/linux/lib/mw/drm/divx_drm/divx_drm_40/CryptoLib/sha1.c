/* $Header: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_40/CryptoLib/sha1.c#1 $
// Copyright (c) 2003 DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivxNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.
*/

#ifndef __KERNEL__

#include "sha1.h"

#else

#include "sha1.h"

#endif

#define SHA_X(a)	\
		*p++ = (unsigned char) (0xFF & (h##a >> 24)); \
		*p++ = (unsigned char) (0xFF & (h##a >> 16)); \
		*p++ = (unsigned char) (0xFF & (h##a >> 8));  \
		*p++ = (unsigned char) (0xFF & (h##a));

#define K1			0x5A827999L
#define K2			0x6ED9EBA1L
#define K3			0x8F1BBCDCL
#define K4			0xCA62C1D6L

#define F1(x,y,z)   ( z ^ ( x & ( y ^ z ) ) )
#define F2(x,y,z)   ( x ^ y ^ z )
#define F3(x,y,z)   ( ( x & y ) | ( z & ( x | y ) ) )
#define F4(x,y,z)   ( x ^ y ^ z )


#define M(i) ( tm =   x[i&0x0f] ^ x[(i-14)&0x0f]            \
		            ^ x[(i-8)&0x0f] ^ x[(i-3)&0x0f]         \
	               , (x[i&0x0f] = (tm << 1) | (tm >> 31)) )


#define rol_5(A) (( A<<5) | ((A>>27) & 0x1F))

#define rol_30(A) (( A<<30) | ((A>>2) & 0x3FFFFFFF))

#define R(a,b,c,d,e,f,k,m)  e += rol_5( a )     \
				      + f( b, c, d )            \
				      + k	                    \
				      + m;	                    \
					  b = rol_30( b ); 


char *sha1GetHash(unsigned char *buf, 
				  unsigned int length, 
				  unsigned int *outlength)
{
	unsigned long h0 = 0L;
	unsigned long h1 = 0L;
	unsigned long h2 = 0L;
	unsigned long h3 = 0L;
	unsigned long h4 = 0L;
	unsigned long nBlocks = 0L;
	unsigned char buffer[64] = {0};
	unsigned int  count = 0;
	unsigned long t = 0L;
	unsigned long msb = 0L;
	unsigned long lsb = 0L;
	unsigned char *p = NULL;

	char *result = (char *) malloc(64);
	if(NULL == result)
	{
		return NULL;
	}

	/*
	 init
	*/

	h0			= 0x67452301;
	h1			= 0xefcdab89;
	h2			= 0x98badcfe;
	h3			= 0x10325476;
	h4			= 0xc3d2e1f0;
	nBlocks	    = 0;
	count       = 0;

	/*
	 Add the data
	*/
	for(count = 0; length >= 64; nBlocks++, length -= 64, buf += 64)
	{
		int i;
	    unsigned long a = h0;
		unsigned long b = h1;
		unsigned long c = h2; 
		unsigned long d = h3;
		unsigned long e = h4;
		unsigned long tm = 0L;
		unsigned long x[16];
		unsigned char *data = buf;

		for(i = 0; i < 16; i++)
		{
            x[i] = (*data << 24) + (*(data+1) << 16) + (*(data+2) << 8) + *(data+3);
            data += 4;
		}

	    R( a, b, c, d, e, F1, K1, x[ 0] );
	    R( e, a, b, c, d, F1, K1, x[ 1] );
	    R( d, e, a, b, c, F1, K1, x[ 2] );
	    R( c, d, e, a, b, F1, K1, x[ 3] );
	    R( b, c, d, e, a, F1, K1, x[ 4] );
	    R( a, b, c, d, e, F1, K1, x[ 5] );
	    R( e, a, b, c, d, F1, K1, x[ 6] );
	    R( d, e, a, b, c, F1, K1, x[ 7] );
	    R( c, d, e, a, b, F1, K1, x[ 8] );
	    R( b, c, d, e, a, F1, K1, x[ 9] );
	    R( a, b, c, d, e, F1, K1, x[10] );
	    R( e, a, b, c, d, F1, K1, x[11] );
	    R( d, e, a, b, c, F1, K1, x[12] );
	    R( c, d, e, a, b, F1, K1, x[13] );
	    R( b, c, d, e, a, F1, K1, x[14] );
	    R( a, b, c, d, e, F1, K1, x[15] );
	    R( e, a, b, c, d, F1, K1, M(16) );
	    R( d, e, a, b, c, F1, K1, M(17) );
	    R( c, d, e, a, b, F1, K1, M(18) );
	    R( b, c, d, e, a, F1, K1, M(19) );
	    R( a, b, c, d, e, F2, K2, M(20) );
	    R( e, a, b, c, d, F2, K2, M(21) );
	    R( d, e, a, b, c, F2, K2, M(22) );
	    R( c, d, e, a, b, F2, K2, M(23) );
	    R( b, c, d, e, a, F2, K2, M(24) );
	    R( a, b, c, d, e, F2, K2, M(25) );
	    R( e, a, b, c, d, F2, K2, M(26) );
	    R( d, e, a, b, c, F2, K2, M(27) );
	    R( c, d, e, a, b, F2, K2, M(28) );
	    R( b, c, d, e, a, F2, K2, M(29) );
	    R( a, b, c, d, e, F2, K2, M(30) );
	    R( e, a, b, c, d, F2, K2, M(31) );
	    R( d, e, a, b, c, F2, K2, M(32) );
	    R( c, d, e, a, b, F2, K2, M(33) );
	    R( b, c, d, e, a, F2, K2, M(34) );
	    R( a, b, c, d, e, F2, K2, M(35) );
	    R( e, a, b, c, d, F2, K2, M(36) );
		R( d, e, a, b, c, F2, K2, M(37) );
	    R( c, d, e, a, b, F2, K2, M(38) );
	    R( b, c, d, e, a, F2, K2, M(39) );
	    R( a, b, c, d, e, F3, K3, M(40) );
	    R( e, a, b, c, d, F3, K3, M(41) );
	    R( d, e, a, b, c, F3, K3, M(42) );
	    R( c, d, e, a, b, F3, K3, M(43) );
	    R( b, c, d, e, a, F3, K3, M(44) );
	    R( a, b, c, d, e, F3, K3, M(45) );
	    R( e, a, b, c, d, F3, K3, M(46) );
	    R( d, e, a, b, c, F3, K3, M(47) );
	    R( c, d, e, a, b, F3, K3, M(48) );
	    R( b, c, d, e, a, F3, K3, M(49) );
	    R( a, b, c, d, e, F3, K3, M(50) );
	    R( e, a, b, c, d, F3, K3, M(51) );
	    R( d, e, a, b, c, F3, K3, M(52) );
	    R( c, d, e, a, b, F3, K3, M(53) );
	    R( b, c, d, e, a, F3, K3, M(54) );
	    R( a, b, c, d, e, F3, K3, M(55) );
	    R( e, a, b, c, d, F3, K3, M(56) );
	    R( d, e, a, b, c, F3, K3, M(57) );
	    R( c, d, e, a, b, F3, K3, M(58) );
	    R( b, c, d, e, a, F3, K3, M(59) );
	    R( a, b, c, d, e, F4, K4, M(60) );
	    R( e, a, b, c, d, F4, K4, M(61) );
	    R( d, e, a, b, c, F4, K4, M(62) );
	    R( c, d, e, a, b, F4, K4, M(63) );
	    R( b, c, d, e, a, F4, K4, M(64) );
	    R( a, b, c, d, e, F4, K4, M(65) );
	    R( e, a, b, c, d, F4, K4, M(66) );
	    R( d, e, a, b, c, F4, K4, M(67) );
	    R( c, d, e, a, b, F4, K4, M(68) );
	    R( b, c, d, e, a, F4, K4, M(69) );
	    R( a, b, c, d, e, F4, K4, M(70) );
	    R( e, a, b, c, d, F4, K4, M(71) );
	    R( d, e, a, b, c, F4, K4, M(72) );
	    R( c, d, e, a, b, F4, K4, M(73) );
	    R( b, c, d, e, a, F4, K4, M(74) );
	    R( a, b, c, d, e, F4, K4, M(75) );
	    R( e, a, b, c, d, F4, K4, M(76) );
	    R( d, e, a, b, c, F4, K4, M(77) );
	    R( c, d, e, a, b, F4, K4, M(78) );
	    R( b, c, d, e, a, F4, K4, M(79) );
	
	    h0 += a;
		h1 += b;
	    h2 += c;
	    h3 += d;
	    h4 += e;
	}

	memcpy(buffer, buf, length);
	count += length;

	/*
	 finalize
	*/

	msb = 0;
	t   = nBlocks;

	if((lsb = t << 6) < t)
	{
		msb++;
	}

	msb += (t >> 26);

	t = lsb;

	if((lsb = t + count) < t)
	{
		msb++;
	}

	t = lsb;

	if((lsb = t << 3) < t)
	{
		msb++;
	}

	msb += (t >> 29);


	if(count != 0)
	{
		if(count < 56) 
		{
			buffer[count++] = 0x80;
			memset(buffer + count, 0, 56 - count);
			count = 56;
		}
		else
		{	
			buffer[count++] = 0x80;

			memset(buffer + count, 0, 64 - count);
			count = 64;

			memset(buffer, 0, 56);
		}
	}

	buffer[56] = (unsigned char) (0xFF & (msb >> 24));
	buffer[57] = (unsigned char) (0xFF & (msb >> 16));
	buffer[58] = (unsigned char) (0xFF & (msb >>  8));
	buffer[59] = (unsigned char) (0xFF & (msb	   ));
	buffer[60] = (unsigned char) (0xFF & (lsb >> 24));
	buffer[61] = (unsigned char) (0xFF & (lsb >> 16));
	buffer[62] = (unsigned char) (0xFF & (lsb >>  8));
	buffer[63] = (unsigned char) (0xFF & (lsb	   ));
	
	{
		int i;
		unsigned char *data = buffer;
	    unsigned long a,b,c,d,e,tm;
	    unsigned long x[16];

	    a = h0;
	    b = h1;
	    c = h2;
	    d = h3;
	    e = h4;

		for(i = 0; i < 16; i++)
		{
            x[i] = (*data << 24) + (*(data+1) << 16) + (*(data+2) << 8) + *(data+3);
            data += 4;
		}

	    R( a, b, c, d, e, F1, K1, x[ 0] );
	    R( e, a, b, c, d, F1, K1, x[ 1] );
	    R( d, e, a, b, c, F1, K1, x[ 2] );
	    R( c, d, e, a, b, F1, K1, x[ 3] );
	    R( b, c, d, e, a, F1, K1, x[ 4] );
	    R( a, b, c, d, e, F1, K1, x[ 5] );
	    R( e, a, b, c, d, F1, K1, x[ 6] );
	    R( d, e, a, b, c, F1, K1, x[ 7] );
	    R( c, d, e, a, b, F1, K1, x[ 8] );
	    R( b, c, d, e, a, F1, K1, x[ 9] );
	    R( a, b, c, d, e, F1, K1, x[10] );
	    R( e, a, b, c, d, F1, K1, x[11] );
	    R( d, e, a, b, c, F1, K1, x[12] );
	    R( c, d, e, a, b, F1, K1, x[13] );
	    R( b, c, d, e, a, F1, K1, x[14] );
	    R( a, b, c, d, e, F1, K1, x[15] );
	    R( e, a, b, c, d, F1, K1, M(16) );
	    R( d, e, a, b, c, F1, K1, M(17) );
	    R( c, d, e, a, b, F1, K1, M(18) );
	    R( b, c, d, e, a, F1, K1, M(19) );
	    R( a, b, c, d, e, F2, K2, M(20) );
	    R( e, a, b, c, d, F2, K2, M(21) );
	    R( d, e, a, b, c, F2, K2, M(22) );
	    R( c, d, e, a, b, F2, K2, M(23) );
	    R( b, c, d, e, a, F2, K2, M(24) );
	    R( a, b, c, d, e, F2, K2, M(25) );
	    R( e, a, b, c, d, F2, K2, M(26) );
	    R( d, e, a, b, c, F2, K2, M(27) );
	    R( c, d, e, a, b, F2, K2, M(28) );
	    R( b, c, d, e, a, F2, K2, M(29) );
	    R( a, b, c, d, e, F2, K2, M(30) );
	    R( e, a, b, c, d, F2, K2, M(31) );
	    R( d, e, a, b, c, F2, K2, M(32) );
	    R( c, d, e, a, b, F2, K2, M(33) );
	    R( b, c, d, e, a, F2, K2, M(34) );
	    R( a, b, c, d, e, F2, K2, M(35) );
	    R( e, a, b, c, d, F2, K2, M(36) );
	    R( d, e, a, b, c, F2, K2, M(37) );
	    R( c, d, e, a, b, F2, K2, M(38) );
	    R( b, c, d, e, a, F2, K2, M(39) );
	    R( a, b, c, d, e, F3, K3, M(40) );
	    R( e, a, b, c, d, F3, K3, M(41) );
	    R( d, e, a, b, c, F3, K3, M(42) );
	    R( c, d, e, a, b, F3, K3, M(43) );
	    R( b, c, d, e, a, F3, K3, M(44) );
	    R( a, b, c, d, e, F3, K3, M(45) );
	    R( e, a, b, c, d, F3, K3, M(46) );
	    R( d, e, a, b, c, F3, K3, M(47) );
	    R( c, d, e, a, b, F3, K3, M(48) );
	    R( b, c, d, e, a, F3, K3, M(49) );
	    R( a, b, c, d, e, F3, K3, M(50) );
	    R( e, a, b, c, d, F3, K3, M(51) );
	    R( d, e, a, b, c, F3, K3, M(52) );
	    R( c, d, e, a, b, F3, K3, M(53) );
	    R( b, c, d, e, a, F3, K3, M(54) );
	    R( a, b, c, d, e, F3, K3, M(55) );
	    R( e, a, b, c, d, F3, K3, M(56) );
	    R( d, e, a, b, c, F3, K3, M(57) );
	    R( c, d, e, a, b, F3, K3, M(58) );
	    R( b, c, d, e, a, F3, K3, M(59) );
	    R( a, b, c, d, e, F4, K4, M(60) );
	    R( e, a, b, c, d, F4, K4, M(61) );
	    R( d, e, a, b, c, F4, K4, M(62) );
	    R( c, d, e, a, b, F4, K4, M(63) );
	    R( b, c, d, e, a, F4, K4, M(64) );
	    R( a, b, c, d, e, F4, K4, M(65) );
	    R( e, a, b, c, d, F4, K4, M(66) );
	    R( d, e, a, b, c, F4, K4, M(67) );
	    R( c, d, e, a, b, F4, K4, M(68) );
	    R( b, c, d, e, a, F4, K4, M(69) );
	    R( a, b, c, d, e, F4, K4, M(70) );
	    R( e, a, b, c, d, F4, K4, M(71) );
	    R( d, e, a, b, c, F4, K4, M(72) );
	    R( c, d, e, a, b, F4, K4, M(73) );
	    R( b, c, d, e, a, F4, K4, M(74) );
	    R( a, b, c, d, e, F4, K4, M(75) );
	    R( e, a, b, c, d, F4, K4, M(76) );
	    R( d, e, a, b, c, F4, K4, M(77) );
	    R( c, d, e, a, b, F4, K4, M(78) );
	    R( b, c, d, e, a, F4, K4, M(79) );
	
	    h0 += a;
	    h1 += b;
	    h2 += c;
	    h3 += d;
	    h4 += e;
	}

	p = (unsigned char *) buffer;

	SHA_X(0);
	SHA_X(1);
	SHA_X(2);
	SHA_X(3);
	SHA_X(4);

	memcpy(result, buffer, 20);
	*outlength = 20;

	return result;
}
