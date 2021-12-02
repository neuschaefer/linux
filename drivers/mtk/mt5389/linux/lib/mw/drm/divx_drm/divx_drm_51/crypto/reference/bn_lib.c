/* crypto/bn/xbn_lib.c */
/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 * 
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 * 
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from 
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 * 
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */

#ifndef XBN_DEBUG
# undef NDEBUG /* avoid conflicting definitions */
# define NDEBUG
#endif

#ifndef __KERNEL__

#ifdef _MTK_BUILD_
#ifndef INT_MAX
#define INT_MAX 2147483647
#endif
#include "libc/stdio.h"
#else
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#endif
//#include "cryptlib.h"
#include "bn_lcl.h"

#ifdef _MTK_BUILD_
#include "libc/memory.h"
#include "libc/stdlib.h"
#else
//#include <memory.h>
#include <stdlib.h>
#endif

#else

#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

#include "bn_lcl.h"

#endif

//const char *XBN_version="Big Number" OPENSSL_VERSION_PTEXT;

/* For a 32 bit machine
 * 2 -   4 ==  128
 * 3 -   8 ==  256
 * 4 -  16 ==  512
 * 5 -  32 == 1024
 * 6 -  64 == 2048
 * 7 - 128 == 4096
 * 8 - 256 == 8192
 */
//static int xbn_limit_bits=0;
//static int xbn_limit_num=8;        /* (1<<xbn_limit_bits) */
//static int xbn_limit_bits_low=0;
//static int xbn_limit_num_low=8;    /* (1<<xbn_limit_bits_low) */
//static int xbn_limit_bits_high=0;
//static int xbn_limit_num_high=8;   /* (1<<xbn_limit_bits_high) */
//static int xbn_limit_bits_mont=0;
//static int xbn_limit_num_mont=8;   /* (1<<xbn_limit_bits_mont) */

void XBN_set_params(XBN_Context* context, int mult, int high, int low, int mont)
	{
   if (context->xbn_limits.xbn_limit_bits == context->xbn_limits.xbn_limit_num == context->xbn_limits.xbn_limit_bits_low ==
         context->xbn_limits.xbn_limit_num_low == context->xbn_limits.xbn_limit_bits_high == 
         context->xbn_limits.xbn_limit_num_high == context->xbn_limits.xbn_limit_bits_mont ==
         context->xbn_limits.xbn_limit_num_mont == 0)
      {
         context->xbn_limits.xbn_limit_bits=0;
         context->xbn_limits.xbn_limit_num=8;
         context->xbn_limits.xbn_limit_bits_low=0;
         context->xbn_limits.xbn_limit_num_low=8;
         context->xbn_limits.xbn_limit_bits_high=0;
         context->xbn_limits.xbn_limit_num_high=8;
         context->xbn_limits.xbn_limit_bits_mont=0;
         context->xbn_limits.xbn_limit_num_mont=8;
      }
	if (mult >= 0)
		{
		if (mult > (sizeof(int)*8)-1)
			mult=sizeof(int)*8-1;
		context->xbn_limits.xbn_limit_bits=mult;
		context->xbn_limits.xbn_limit_num=1<<mult;
		}
	if (high >= 0)
		{
		if (high > (sizeof(int)*8)-1)
			high=sizeof(int)*8-1;
		context->xbn_limits.xbn_limit_bits_high=high;
		context->xbn_limits.xbn_limit_num_high=1<<high;
		}
	if (low >= 0)
		{
		if (low > (sizeof(int)*8)-1)
			low=sizeof(int)*8-1;
		context->xbn_limits.xbn_limit_bits_low=low;
		context->xbn_limits.xbn_limit_num_low=1<<low;
		}
	if (mont >= 0)
		{
		if (mont > (sizeof(int)*8)-1)
			mont=sizeof(int)*8-1;
		context->xbn_limits.xbn_limit_bits_mont=mont;
		context->xbn_limits.xbn_limit_num_mont=1<<mont;
		}
	}

int XBN_get_params(XBN_Context* context, int which)
	{
	if      (which == 0) return(context->xbn_limits.xbn_limit_bits);
	else if (which == 1) return(context->xbn_limits.xbn_limit_bits_high);
	else if (which == 2) return(context->xbn_limits.xbn_limit_bits_low);
	else if (which == 3) return(context->xbn_limits.xbn_limit_bits_mont);
	else return(0);
	}

const XBIGNUM *XBN_value_one(void)
	{
	static const XBN_ULONG data_one=1L;
	static const XBIGNUM const_one={(unsigned long *)&data_one,1,1,0};

	return(&const_one);
	}



void XBN_init_options(XBN_Context* context)
    {
    context->init = 0;
    }

char *XBN_options(XBN_Context* context)
	{

	if (!context->init)
		{
		context->init++;
#ifdef XBN_LLONG
		sprintf(context->data,"bn(%d,%d)",(int)sizeof(XBN_ULLONG)*8,
			(int)sizeof(XBN_ULONG)*8);
#else
		sprintf(context->data,"bn(%d,%d)",(int)sizeof(XBN_ULONG)*8,
			(int)sizeof(XBN_ULONG)*8);
#endif
		}
	return(context->data);
	}

int XBN_num_bits_word(XBN_ULONG l)
	{
	static const char bits[256]={
		0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		};

#if defined(SIXTY_FOUR_BIT_LONG)
	if (l & 0xffffffff00000000L)
		{
		if (l & 0xffff000000000000L)
			{
			if (l & 0xff00000000000000L)
				{
				return(bits[(int)(l>>56)]+56);
				}
			else	return(bits[(int)(l>>48)]+48);
			}
		else
			{
			if (l & 0x0000ff0000000000L)
				{
				return(bits[(int)(l>>40)]+40);
				}
			else	return(bits[(int)(l>>32)]+32);
			}
		}
	else
#else
#ifdef SIXTY_FOUR_BIT
	if (l & 0xffffffff00000000LL)
		{
		if (l & 0xffff000000000000LL)
			{
			if (l & 0xff00000000000000LL)
				{
				return(bits[(int)(l>>56)]+56);
				}
			else	return(bits[(int)(l>>48)]+48);
			}
		else
			{
			if (l & 0x0000ff0000000000LL)
				{
				return(bits[(int)(l>>40)]+40);
				}
			else	return(bits[(int)(l>>32)]+32);
			}
		}
	else
#endif
#endif
		{
#if defined(THIRTY_TWO_BIT) || defined(SIXTY_FOUR_BIT) || defined(SIXTY_FOUR_BIT_LONG)
		if (l & 0xffff0000L)
			{
			if (l & 0xff000000L)
				return(bits[(int)(l>>24L)]+24);
			else	return(bits[(int)(l>>16L)]+16);
			}
		else
#endif
			{
#if defined(SIXTEEN_BIT) || defined(THIRTY_TWO_BIT) || defined(SIXTY_FOUR_BIT) || defined(SIXTY_FOUR_BIT_LONG)
			if (l & 0xff00L)
				return(bits[(int)(l>>8)]+8);
			else	
#endif
				return(bits[(int)(l   )]  );
			}
		}
	}

int XBN_num_bits(const XBIGNUM *a)
	{
	XBN_ULONG l;
	int i;

	xbn_check_top(a);

	if (a->top == 0) return(0);
	l=a->d[a->top-1];
	assert(l != 0);
	i=(a->top-1)*XBN_BITS2;
	return(i+XBN_num_bits_word(l));
	}

void XBN_clear_free(XBIGNUM *a)
	{
	int i;

	if (a == NULL) return;
	if (a->d != NULL)
		{
		memset(a->d,0,a->dmax*sizeof(a->d[0]));
		if (!(XBN_get_flags(a,XBN_FLG_STATIC_DATA)))
		{
			//OPENSSL_free(a->d);
			free(a->d);
		}
			
		}
	i=XBN_get_flags(a,XBN_FLG_MALLOCED);
	memset(a,0,sizeof(XBIGNUM));
	if (i)
	{
		//OPENSSL_free(a);
		free(a->d);
	}
		
	}

void XBN_free(XBIGNUM *a)
	{
	if (a == NULL) return;
	if ((a->d != NULL) && !(XBN_get_flags(a,XBN_FLG_STATIC_DATA)))
	{
		//OPENSSL_free(a->d);
		free(a->d);
	}		
	a->flags|=XBN_FLG_FREE; /* REMOVE? */
	if (a->flags & XBN_FLG_MALLOCED)
	{
		//OPENSSL_free(a);
		free(a);
	}		
	}

void XBN_init(XBIGNUM *a)
	{
	memset(a,0,sizeof(XBIGNUM));
	}

XBIGNUM *XBN_new(void)
	{
	XBIGNUM *ret;

	//if ((ret=(XBIGNUM *)OPENSSL_malloc(sizeof(XBIGNUM))) == NULL)
	if ((ret=(XBIGNUM *)malloc(sizeof(XBIGNUM))) == NULL)
		{
		//BNerr(XBN_F_XBN_NEW,ERR_R_MALLOC_FAILURE);
		return(NULL);
		}
	ret->flags=XBN_FLG_MALLOCED;
	ret->top=0;
	ret->neg=0;
	ret->dmax=0;
	ret->d=NULL;
	return(ret);
	}

/* This is used both by xbn_expand2() and xbn_dup_expand() */
/* The caller MUST check that words > b->dmax before calling this */
static XBN_ULONG *xbn_expand_internal(const XBIGNUM *b, int words)
	{
	XBN_ULONG *A,*a = NULL;
	const XBN_ULONG *B;
	int i;

	if (words > (INT_MAX/(4*XBN_BITS2)))
		{
		//BNerr(XBN_F_XBN_EXPAND_INTERNAL,XBN_R_XBIGNUM_TOO_LONG);
		return NULL;
		}

	xbn_check_top(b);	
	if (XBN_get_flags(b,XBN_FLG_STATIC_DATA))
		{
		//BNerr(XBN_F_XBN_EXPAND_INTERNAL,XBN_R_EXPAND_ON_STATIC_XBIGNUM_DATA);
		return(NULL);
		}
	//a=A=(XBN_ULONG *)OPENSSL_malloc(sizeof(XBN_ULONG)*(words+1));
	a=A=(XBN_ULONG *)malloc(sizeof(XBN_ULONG)*(words+1));
	if (A == NULL)
		{
		//BNerr(XBN_F_XBN_EXPAND_INTERNAL,ERR_R_MALLOC_FAILURE);
		return(NULL);
		}
#if 1
	B=b->d;
	/* Check if the previous number needs to be copied */
	if (B != NULL)
		{
		for (i=b->top>>2; i>0; i--,A+=4,B+=4)
			{
			/*
			 * The fact that the loop is unrolled
			 * 4-wise is a tribute to Intel. It's
			 * the one that doesn't have enough
			 * registers to accomodate more data.
			 * I'd unroll it 8-wise otherwise:-)
			 *
			 *		<appro@fy.chalmers.se>
			 */
			XBN_ULONG a0,a1,a2,a3;
			a0=B[0]; a1=B[1]; a2=B[2]; a3=B[3];
			A[0]=a0; A[1]=a1; A[2]=a2; A[3]=a3;
			}
		switch (b->top&3)
			{
		case 3:	A[2]=B[2];
		case 2:	A[1]=B[1];
		case 1:	A[0]=B[0];
		case 0: /* workaround for ultrix cc: without 'case 0', the optimizer does
		         * the switch table by doing a=top&3; a--; goto jump_table[a];
		         * which fails for top== 0 */
			;
			}
		}

	/* Now need to zero any data between b->top and b->max */
	/* XXX Why? */

	A= &(a[b->top]);
	for (i=(words - b->top)>>3; i>0; i--,A+=8)
		{
		A[0]=0; A[1]=0; A[2]=0; A[3]=0;
		A[4]=0; A[5]=0; A[6]=0; A[7]=0;
		}
	for (i=(words - b->top)&7; i>0; i--,A++)
		A[0]=0;
#else
	memset(A,0,sizeof(XBN_ULONG)*(words+1));
	memcpy(A,b->d,sizeof(b->d[0])*b->top);
#endif
		
	return(a);
	}

/* This is an internal function that can be used instead of xbn_expand2()
 * when there is a need to copy XBIGNUMs instead of only expanding the
 * data part, while still expanding them.
 * Especially useful when needing to expand XBIGNUMs that are declared
 * 'const' and should therefore not be changed.
 * The reason to use this instead of a XBN_dup() followed by a xbn_expand2()
 * is memory allocation overhead.  A XBN_dup() followed by a xbn_expand2()
 * will allocate new memory for the XBIGNUM data twice, and free it once,
 * while xbn_dup_expand() makes sure allocation is made only once.
 */

XBIGNUM *xbn_dup_expand(const XBIGNUM *b, int words)
	{
	XBIGNUM *r = NULL;

	if (words > b->dmax)
		{
		XBN_ULONG *a = xbn_expand_internal(b, words);

		if (a)
			{
			r = XBN_new();
			if (r)
				{
				r->top = b->top;
				r->dmax = words;
				r->neg = b->neg;
				r->d = a;
				}
			else
				{
				/* r == NULL, XBN_new failure */
				//OPENSSL_free(a);
				free(a);
				}
			}
		/* If a == NULL, there was an error in allocation in
		   xbn_expand_internal(), and NULL should be returned */
		}
	else
		{
		r = XBN_dup(b);
		}

	return r;
	}

/* This is an internal function that should not be used in applications.
 * It ensures that 'b' has enough room for a 'words' word number number.
 * It is mostly used by the various XBIGNUM routines. If there is an error,
 * NULL is returned. If not, 'b' is returned. */

XBIGNUM *xbn_expand2(XBIGNUM *b, int words)
	{
	if (words > b->dmax)
		{
		XBN_ULONG *a = xbn_expand_internal(b, words);

		if (a)
			{
			if (b->d)
			{
				//OPENSSL_free(b->d);
				free(b->d);
			}
			
			b->d=a;
			b->dmax=words;
			}
		else
			b = NULL;
		}
	return b;
	}

XBIGNUM *XBN_dup(const XBIGNUM *a)
	{
	XBIGNUM *r, *t;

	if (a == NULL) return NULL;

	xbn_check_top(a);

	t = XBN_new();
	if (t == NULL) return(NULL);
	r = XBN_copy(t, a);
	/* now  r == t || r == NULL */
	if (r == NULL)
		XBN_free(t);
	return r;
	}

XBIGNUM *XBN_copy(XBIGNUM *a, const XBIGNUM *b)
	{
	int i;
	XBN_ULONG *A;
	const XBN_ULONG *B;

	xbn_check_top(b);

	if (a == b) return(a);
	if (xbn_wexpand(a,b->top) == NULL) return(NULL);

#if 1
	A=a->d;
	B=b->d;
	for (i=b->top>>2; i>0; i--,A+=4,B+=4)
		{
		XBN_ULONG a0,a1,a2,a3;
		a0=B[0]; a1=B[1]; a2=B[2]; a3=B[3];
		A[0]=a0; A[1]=a1; A[2]=a2; A[3]=a3;
		}
	switch (b->top&3)
		{
		case 3: A[2]=B[2];
		case 2: A[1]=B[1];
		case 1: A[0]=B[0];
		case 0: ; /* ultrix cc workaround, see comments in xbn_expand_internal */
		}
#else
	memcpy(a->d,b->d,sizeof(b->d[0])*b->top);
#endif

/*	memset(&(a->d[b->top]),0,sizeof(a->d[0])*(a->max-b->top));*/
	a->top=b->top;
	if ((a->top == 0) && (a->d != NULL))
		a->d[0]=0;
	a->neg=b->neg;
	return(a);
	}

void XBN_swap(XBIGNUM *a, XBIGNUM *b)
	{
	int flags_old_a, flags_old_b;
	XBN_ULONG *tmp_d;
	int tmp_top, tmp_dmax, tmp_neg;
	
	flags_old_a = a->flags;
	flags_old_b = b->flags;

	tmp_d = a->d;
	tmp_top = a->top;
	tmp_dmax = a->dmax;
	tmp_neg = a->neg;
	
	a->d = b->d;
	a->top = b->top;
	a->dmax = b->dmax;
	a->neg = b->neg;
	
	b->d = tmp_d;
	b->top = tmp_top;
	b->dmax = tmp_dmax;
	b->neg = tmp_neg;
	
	a->flags = (flags_old_a & XBN_FLG_MALLOCED) | (flags_old_b & XBN_FLG_STATIC_DATA);
	b->flags = (flags_old_b & XBN_FLG_MALLOCED) | (flags_old_a & XBN_FLG_STATIC_DATA);
	}


void XBN_clear(XBIGNUM *a)
	{
	if (a->d != NULL)
		memset(a->d,0,a->dmax*sizeof(a->d[0]));
	a->top=0;
	a->neg=0;
	}

XBN_ULONG XBN_get_word(const XBIGNUM *a)
	{
	int i,n;
	XBN_ULONG ret=0;

	n=XBN_num_bytes(a);
	if (n > sizeof(XBN_ULONG))
		return(XBN_MASK2);
	for (i=a->top-1; i>=0; i--)
		{
#ifndef SIXTY_FOUR_BIT /* the data item > unsigned long */
		ret<<=XBN_BITS4; /* stops the compiler complaining */
		ret<<=XBN_BITS4;
#else
		ret=0;
#endif
		ret|=a->d[i];
		}
	return(ret);
	}

int XBN_set_word(XBIGNUM *a, XBN_ULONG w)
	{
	int i,n;
	if (xbn_expand(a,sizeof(XBN_ULONG)*8) == NULL) return(0);

	n=sizeof(XBN_ULONG)/XBN_BYTES;
	a->neg=0;
	a->top=0;
	a->d[0]=(XBN_ULONG)w&XBN_MASK2;
	if (a->d[0] != 0) a->top=1;
	for (i=1; i<n; i++)
		{
		/* the following is done instead of
		 * w>>=XBN_BITS2 so compilers don't complain
		 * on builds where sizeof(long) == XBN_TYPES */
#ifndef SIXTY_FOUR_BIT /* the data item > unsigned long */
		w>>=XBN_BITS4;
		w>>=XBN_BITS4;
#else
		w=0;
#endif
		a->d[i]=(XBN_ULONG)w&XBN_MASK2;
		if (a->d[i] != 0) a->top=i+1;
		}
	return(1);
	}

XBIGNUM *XBN_bin2bn(const unsigned char *s, int len, XBIGNUM *ret)
	{
	unsigned int i,m;
	unsigned int n;
	XBN_ULONG l;

	if (ret == NULL) ret=XBN_new();
	if (ret == NULL) return(NULL);
	l=0;
	n=len;
	if (n == 0)
		{
		ret->top=0;
		return(ret);
		}
	if (xbn_expand(ret,(int)(n+2)*8) == NULL)
		return(NULL);
	i=((n-1)/XBN_BYTES)+1;
	m=((n-1)%(XBN_BYTES));
	ret->top=i;
	ret->neg=0;
	while (n-- > 0)
		{
		l=(l<<8L)| *(s++);
		if (m-- == 0)
			{
			ret->d[--i]=l;
			l=0;
			m=XBN_BYTES-1;
			}
		}
	/* need to call this due to clear byte at top if avoiding
	 * having the top bit set (-ve number) */
	xbn_fix_top(ret);
	return(ret);
	}

/* ignore negative */
int XBN_bn2bin(const XBIGNUM *a, unsigned char *to)
	{
	int n,i;
	XBN_ULONG l;
	int result; //hmaxf ,,, additional variable to prevent warning at level 4

	n=i=XBN_num_bytes(a);
	while (i-- > 0)
		{
		l=a->d[i/XBN_BYTES];
		result = (l>>(8*(i %  XBN_BYTES))) & 0xff;
		*(to++)=(unsigned char) result;
		}
	return(n);
	}

int XBN_ucmp(const XBIGNUM *a, const XBIGNUM *b)
	{
	int i;
	XBN_ULONG t1,t2,*ap,*bp;

	xbn_check_top(a);
	xbn_check_top(b);

	i=a->top-b->top;
	if (i != 0) return(i);
	ap=a->d;
	bp=b->d;
	for (i=a->top-1; i>=0; i--)
		{
		t1= ap[i];
		t2= bp[i];
		if (t1 != t2)
			return(t1 > t2?1:-1);
		}
	return(0);
	}

int XBN_cmp(const XBIGNUM *a, const XBIGNUM *b)
	{
	int i;
	int gt,lt;
	XBN_ULONG t1,t2;

	if ((a == NULL) || (b == NULL))
		{
		if (a != NULL)
			return(-1);
		else if (b != NULL)
			return(1);
		else
			return(0);
		}

	xbn_check_top(a);
	xbn_check_top(b);

	if (a->neg != b->neg)
		{
		if (a->neg)
			return(-1);
		else	return(1);
		}
	if (a->neg == 0)
		{ gt=1; lt= -1; }
	else	{ gt= -1; lt=1; }

	if (a->top > b->top) return(gt);
	if (a->top < b->top) return(lt);
	for (i=a->top-1; i>=0; i--)
		{
		t1=a->d[i];
		t2=b->d[i];
		if (t1 > t2) return(gt);
		if (t1 < t2) return(lt);
		}
	return(0);
	}

int XBN_set_bit(XBIGNUM *a, int n)
	{
	int i,j,k;

	i=n/XBN_BITS2;
	j=n%XBN_BITS2;
	if (a->top <= i)
		{
		if (xbn_wexpand(a,i+1) == NULL) return(0);
		for(k=a->top; k<i+1; k++)
			a->d[k]=0;
		a->top=i+1;
		}

	a->d[i]|=(((XBN_ULONG)1)<<j);
	return(1);
	}

int XBN_clear_bit(XBIGNUM *a, int n)
	{
	int i,j;

	i=n/XBN_BITS2;
	j=n%XBN_BITS2;
	if (a->top <= i) return(0);

	a->d[i]&=(~(((XBN_ULONG)1)<<j));
	xbn_fix_top(a);
	return(1);
	}

int XBN_is_bit_set(const XBIGNUM *a, int n)
	{
	int i,j;

	if (n < 0) return(0);
	i=n/XBN_BITS2;
	j=n%XBN_BITS2;
	if (a->top <= i) return(0);
	return((a->d[i]&(((XBN_ULONG)1)<<j))?1:0);
	}

int XBN_mask_bits(XBIGNUM *a, int n)
	{
	int b,w;

	w=n/XBN_BITS2;
	b=n%XBN_BITS2;
	if (w >= a->top) return(0);
	if (b == 0)
		a->top=w;
	else
		{
		a->top=w+1;
		a->d[w]&= ~(XBN_MASK2<<b);
		}
	xbn_fix_top(a);
	return(1);
	}

int xbn_cmp_words(const XBN_ULONG *a, const XBN_ULONG *b, int n)
	{
	int i;
	XBN_ULONG aa,bb;

	aa=a[n-1];
	bb=b[n-1];
	if (aa != bb) return((aa > bb)?1:-1);
	for (i=n-2; i>=0; i--)
		{
		aa=a[i];
		bb=b[i];
		if (aa != bb) return((aa > bb)?1:-1);
		}
	return(0);
	}

/* Here follows a specialised variants of xbn_cmp_words().  It has the
   property of performing the operation on arrays of different sizes.
   The sizes of those arrays is expressed through cl, which is the
   common length ( basicall, min(len(a),len(b)) ), and dl, which is the
   delta between the two lengths, calculated as len(a)-len(b).
   All lengths are the number of XBN_ULONGs...  */

int xbn_cmp_part_words(const XBN_ULONG *a, const XBN_ULONG *b,
	int cl, int dl)
	{
	int n,i;
	n = cl-1;

	if (dl < 0)
		{
		for (i=dl; i<0; i++)
			{
			if (b[n-i] != 0)
				return -1; /* a < b */
			}
		}
	if (dl > 0)
		{
		for (i=dl; i>0; i--)
			{
			if (a[n+i] != 0)
				return 1; /* a > b */
			}
		}
	return xbn_cmp_words(a,b,cl);
	}
