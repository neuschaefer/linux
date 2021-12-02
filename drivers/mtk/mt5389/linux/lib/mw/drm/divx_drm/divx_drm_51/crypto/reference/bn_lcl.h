/* crypto/bn/bn_lcl.h */
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
/* ====================================================================
 * Copyright (c) 1998-2000 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */

#ifndef HEADER_BN_LCL_H
#define HEADER_BN_LCL_H

#ifndef __KERNEL__

//#include <openssl/bn.h>
#include "bn.h"
#ifdef _MTK_BUILD_
#include "libc/stdlib.h"
#include "libc/memory.h"
#else
#include <stdlib.h>
//#include <memory.h>
#endif

#else

#include "bn.h"

#endif

#ifdef  __cplusplus
extern "C" {
#endif


/* Used for temp variables */
#define XBN_CTX_NUM	32
#define XBN_CTX_NUM_POS	12
struct xbignum_ctx
	{
	int tos;
	XBIGNUM bn[XBN_CTX_NUM];
	int flags;
	int depth;
	int pos[XBN_CTX_NUM_POS];
	int too_many;
	} /* XBN_CTX */;


/*
 * XBN_window_bits_for_exponent_size -- macro for sliding window mod_exp functions
 *
 *
 * For window size 'w' (w >= 2) and a random 'b' bits exponent,
 * the number of multiplications is a constant plus on average
 *
 *    2^(w-1) + (b-w)/(w+1);
 *
 * here  2^(w-1)  is for precomputing the table (we actually need
 * entries only for windows that have the lowest bit set), and
 * (b-w)/(w+1)  is an approximation for the expected number of
 * w-bit windows, not counting the first one.
 *
 * Thus we should use
 *
 *    w >= 6  if        b > 671
 *     w = 5  if  671 > b > 239
 *     w = 4  if  239 > b >  79
 *     w = 3  if   79 > b >  23
 *    w <= 2  if   23 > b
 *
 * (with draws in between).  Very small exponents are often selected
 * with low Hamming weight, so we use  w = 1  for b <= 23.
 */
#if 1
#define XBN_window_bits_for_exponent_size(b) \
		((b) > 671 ? 6 : \
		 (b) > 239 ? 5 : \
		 (b) >  79 ? 4 : \
		 (b) >  23 ? 3 : 1)
#else
/* Old SSLeay/OpenSSL table.
 * Maximum window size was 5, so this table differs for b==1024;
 * but it coincides for other interesting values (b==160, b==512).
 */
#define XBN_window_bits_for_exponent_size(b) \
		((b) > 255 ? 5 : \
		 (b) > 127 ? 4 : \
		 (b) >  17 ? 3 : 1)
#endif	 



/* Pentium pro 16,16,16,32,64 */
/* Alpha       16,16,16,16.64 */
#define XBN_MULL_SIZE_NORMAL			(16) /* 32 */
#define XBN_MUL_RECURSIVE_SIZE_NORMAL		(16) /* 32 less than */
#define XBN_SQR_RECURSIVE_SIZE_NORMAL		(16) /* 32 */
#define XBN_MUL_LOW_RECURSIVE_SIZE_NORMAL	(32) /* 32 */
#define XBN_MONT_CTX_SET_SIZE_WORD		(64) /* 32 */

#if !defined(OPENSSL_NO_ASM) && !defined(OPENSSL_NO_INLINE_ASM) && !defined(PEDANTIC)
/*
 * XBN_UMULT_HIGH section.
 *
 * No, I'm not trying to overwhelm you when stating that the
 * product of N-bit numbers is 2*N bits wide:-) No, I don't expect
 * you to be impressed when I say that if the compiler doesn't
 * support 2*N integer type, then you have to replace every N*N
 * multiplication with 4 (N/2)*(N/2) accompanied by some shifts
 * and additions which unavoidably results in severe performance
 * penalties. Of course provided that the hardware is capable of
 * producing 2*N result... That's when you normally start
 * considering assembler implementation. However! It should be
 * pointed out that some CPUs (most notably Alpha, PowerPC and
 * upcoming IA-64 family:-) provide *separate* instruction
 * calculating the upper half of the product placing the result
 * into a general purpose register. Now *if* the compiler supports
 * inline assembler, then it's not impossible to implement the
 * "bignum" routines (and have the compiler optimize 'em)
 * exhibiting "native" performance in C. That's what XBN_UMULT_HIGH
 * macro is about:-)
 *
 *					<appro@fy.chalmers.se>
 */
# if defined(__alpha) && (defined(SIXTY_FOUR_BIT_LONG) || defined(SIXTY_FOUR_BIT))
#  if defined(__DECC)
#   include <c_asm.h>
#   define XBN_UMULT_HIGH(a,b)	(XBN_ULONG)asm("umulh %a0,%a1,%v0",(a),(b))
#  elif defined(__GNUC__)
#   define XBN_UMULT_HIGH(a,b)	({	\
	register XBN_ULONG ret;		\
	asm ("umulh	%1,%2,%0"	\
	     : "=r"(ret)		\
	     : "r"(a), "r"(b));		\
	ret;			})
#  endif	/* compiler */
# elif defined(_ARCH_PPC) && defined(__64BIT__) && defined(SIXTY_FOUR_BIT_LONG)
#  if defined(__GNUC__)
#   define XBN_UMULT_HIGH(a,b)	({	\
	register XBN_ULONG ret;		\
	asm ("mulhdu	%0,%1,%2"	\
	     : "=r"(ret)		\
	     : "r"(a), "r"(b));		\
	ret;			})
#  endif	/* compiler */
# endif		/* cpu */
#endif		/* OPENSSL_NO_ASM */

/*************************************************************
 * Using the long long type
 */
#define Lw(t)    (((XBN_ULONG)(t))&XBN_MASK2)
#define Hw(t)    (((XBN_ULONG)((t)>>XBN_BITS2))&XBN_MASK2)

/* This is used for internal error checking and is not normally used */
#ifdef XBN_DEBUG
# include <assert.h>
# define xbn_check_top(a) assert ((a)->top >= 0 && (a)->top <= (a)->dmax);
#else
# define xbn_check_top(a)
#endif

/* This macro is to add extra stuff for development checking */
#ifdef XBN_DEBUG
#define	xbn_set_max(r) ((r)->max=(r)->top,XBN_set_flags((r),XBN_FLG_STATIC_DATA))
#else
#define	xbn_set_max(r)
#endif

/* These macros are used to 'take' a section of a bignum for read only use */
#define xbn_set_low(r,a,n) \
	{ \
	(r)->top=((a)->top > (n))?(n):(a)->top; \
	(r)->d=(a)->d; \
	(r)->neg=(a)->neg; \
	(r)->flags|=XBN_FLG_STATIC_DATA; \
	xbn_set_max(r); \
	}

#define xbn_set_high(r,a,n) \
	{ \
	if ((a)->top > (n)) \
		{ \
		(r)->top=(a)->top-n; \
		(r)->d= &((a)->d[n]); \
		} \
	else \
		(r)->top=0; \
	(r)->neg=(a)->neg; \
	(r)->flags|=XBN_FLG_STATIC_DATA; \
	xbn_set_max(r); \
	}

#ifdef XBN_LLONG
#define mul_add(r,a,w,c) { \
	XBN_ULLONG t; \
	t=(XBN_ULLONG)w * (a) + (r) + (c); \
	(r)= Lw(t); \
	(c)= Hw(t); \
	}

#define mul(r,a,w,c) { \
	XBN_ULLONG t; \
	t=(XBN_ULLONG)w * (a) + (c); \
	(r)= Lw(t); \
	(c)= Hw(t); \
	}

#define sqr(r0,r1,a) { \
	XBN_ULLONG t; \
	t=(XBN_ULLONG)(a)*(a); \
	(r0)=Lw(t); \
	(r1)=Hw(t); \
	}

#elif defined(XBN_UMULT_HIGH)
#define mul_add(r,a,w,c) {		\
	XBN_ULONG high,low,ret,tmp=(a);	\
	ret =  (r);			\
	high=  XBN_UMULT_HIGH(w,tmp);	\
	ret += (c);			\
	low =  (w) * tmp;		\
	(c) =  (ret<(c))?1:0;		\
	(c) += high;			\
	ret += low;			\
	(c) += (ret<low)?1:0;		\
	(r) =  ret;			\
	}

#define mul(r,a,w,c)	{		\
	XBN_ULONG high,low,ret,ta=(a);	\
	low =  (w) * ta;		\
	high=  XBN_UMULT_HIGH(w,ta);	\
	ret =  low + (c);		\
	(c) =  high;			\
	(c) += (ret<low)?1:0;		\
	(r) =  ret;			\
	}

#define sqr(r0,r1,a)	{		\
	XBN_ULONG tmp=(a);		\
	(r0) = tmp * tmp;		\
	(r1) = XBN_UMULT_HIGH(tmp,tmp);	\
	}

#else
/*************************************************************
 * No long long type
 */

#define LBITS(a)	((a)&XBN_MASK2l)
#define HBITS(a)	(((a)>>XBN_BITS4)&XBN_MASK2l)
#define	L2HBITS(a)	((XBN_ULONG)((a)&XBN_MASK2l)<<XBN_BITS4)

#define LLBITS(a)	((a)&XBN_MASKl)
#define LHBITS(a)	(((a)>>XBN_BITS2)&XBN_MASKl)
#define	LL2HBITS(a)	((XBN_ULLONG)((a)&XBN_MASKl)<<XBN_BITS2)

#define mul64(l,h,bl,bh) \
	{ \
	XBN_ULONG m,m1,lt,ht; \
 \
	lt=l; \
	ht=h; \
	m =(bh)*(lt); \
	lt=(bl)*(lt); \
	m1=(bl)*(ht); \
	ht =(bh)*(ht); \
	m=(m+m1)&XBN_MASK2; if (m < m1) ht+=L2HBITS(1L); \
	ht+=HBITS(m); \
	m1=L2HBITS(m); \
	lt=(lt+m1)&XBN_MASK2; if (lt < m1) ht++; \
	(l)=lt; \
	(h)=ht; \
	}

#define sqr64(lo,ho,in) \
	{ \
	XBN_ULONG l,h,m; \
 \
	h=(in); \
	l=LBITS(h); \
	h=HBITS(h); \
	m =(l)*(h); \
	l*=l; \
	h*=h; \
	h+=(m&XBN_MASK2h1)>>(XBN_BITS4-1); \
	m =(m&XBN_MASK2l)<<(XBN_BITS4+1); \
	l=(l+m)&XBN_MASK2; if (l < m) h++; \
	(lo)=l; \
	(ho)=h; \
	}

#define mul_add(r,a,bl,bh,c) { \
	XBN_ULONG l,h; \
 \
	h= (a); \
	l=LBITS(h); \
	h=HBITS(h); \
	mul64(l,h,(bl),(bh)); \
 \
	/* non-multiply part */ \
	l=(l+(c))&XBN_MASK2; if (l < (c)) h++; \
	(c)=(r); \
	l=(l+(c))&XBN_MASK2; if (l < (c)) h++; \
	(c)=h&XBN_MASK2; \
	(r)=l; \
	}

#define mul(r,a,bl,bh,c) { \
	XBN_ULONG l,h; \
 \
	h= (a); \
	l=LBITS(h); \
	h=HBITS(h); \
	mul64(l,h,(bl),(bh)); \
 \
	/* non-multiply part */ \
	l+=(c); if ((l&XBN_MASK2) < (c)) h++; \
	(c)=h&XBN_MASK2; \
	(r)=l&XBN_MASK2; \
	}
#endif /* !XBN_LLONG */

void xbn_mul_normal(XBN_ULONG *r,XBN_ULONG *a,int na,XBN_ULONG *b,int nb);
void xbn_mul_comba8(XBN_ULONG *r,XBN_ULONG *a,XBN_ULONG *b);
void xbn_mul_comba4(XBN_ULONG *r,XBN_ULONG *a,XBN_ULONG *b);
void xbn_sqr_normal(XBN_ULONG *r, const XBN_ULONG *a, int n, XBN_ULONG *tmp);
void xbn_sqr_comba8(XBN_ULONG *r,const XBN_ULONG *a);
void xbn_sqr_comba4(XBN_ULONG *r,const XBN_ULONG *a);
int xbn_cmp_words(const XBN_ULONG *a,const XBN_ULONG *b,int n);
int xbn_cmp_part_words(const XBN_ULONG *a, const XBN_ULONG *b,
	int cl, int dl);
void xbn_mul_recursive(XBN_ULONG *r,XBN_ULONG *a,XBN_ULONG *b,int n2,
	int dna,int dnb,XBN_ULONG *t);
void xbn_mul_part_recursive(XBN_ULONG *r,XBN_ULONG *a,XBN_ULONG *b,
	int n,int tna,int tnb,XBN_ULONG *t);
void xbn_sqr_recursive(XBN_ULONG *r,const XBN_ULONG *a, int n2, XBN_ULONG *t);
void xbn_mul_low_normal(XBN_ULONG *r,XBN_ULONG *a,XBN_ULONG *b, int n);
void xbn_mul_low_recursive(XBN_ULONG *r,XBN_ULONG *a,XBN_ULONG *b,int n2,
	XBN_ULONG *t);
void xbn_mul_high(XBN_ULONG *r,XBN_ULONG *a,XBN_ULONG *b,XBN_ULONG *l,int n2,
	XBN_ULONG *t);
XBN_ULONG xbn_add_part_words(XBN_ULONG *r, const XBN_ULONG *a, const XBN_ULONG *b,
	int cl, int dl);
XBN_ULONG xbn_sub_part_words(XBN_ULONG *r, const XBN_ULONG *a, const XBN_ULONG *b,
	int cl, int dl);

#ifdef  __cplusplus
}
#endif

#endif
