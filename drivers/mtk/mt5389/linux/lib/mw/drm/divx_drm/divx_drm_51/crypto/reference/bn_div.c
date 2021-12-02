/* crypto/bn/xbn_div.c */
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

#ifndef __KERNEL__

#ifdef _MTK_BUILD_
#include "libc/stdio.h"
#else
#include <stdio.h>
#endif
//#include <openssl/bn.h>
#include "bn.h"
//#include "cryptlib.h"
#include "bn_lcl.h"

#else

#include "bn.h"
#include "bn_lcl.h"

#endif

/* The old slow way */
#if 0
int XBN_div(XBIGNUM *dv, XBIGNUM *rem, const XBIGNUM *m, const XBIGNUM *d,
	   XBN_CTX *ctx)
	{
	int i,nm,nd;
	int ret = 0;
	XBIGNUM *D;

	xbn_check_top(m);
	xbn_check_top(d);
	if (XBN_is_zero(d))
		{
		//BNerr(XBN_F_XBN_DIV,XBN_R_DIV_BY_ZERO);
		return(0);
		}

	if (XBN_ucmp(m,d) < 0)
		{
		if (rem != NULL)
			{ if (XBN_copy(rem,m) == NULL) return(0); }
		if (dv != NULL) XBN_zero(dv);
		return(1);
		}

	XBN_CTX_start(ctx);
	D = XBN_CTX_get(ctx);
	if (dv == NULL) dv = XBN_CTX_get(ctx);
	if (rem == NULL) rem = XBN_CTX_get(ctx);
	if (D == NULL || dv == NULL || rem == NULL)
		goto end;

	nd=XBN_num_bits(d);
	nm=XBN_num_bits(m);
	if (XBN_copy(D,d) == NULL) goto end;
	if (XBN_copy(rem,m) == NULL) goto end;

	/* The next 2 are needed so we can do a dv->d[0]|=1 later
	 * since XBN_lshift1 will only work once there is a value :-) */
	XBN_zero(dv);
	xbn_wexpand(dv,1);
	dv->top=1;

	if (!XBN_lshift(D,D,nm-nd)) goto end;
	for (i=nm-nd; i>=0; i--)
		{
		if (!XBN_lshift1(dv,dv)) goto end;
		if (XBN_ucmp(rem,D) >= 0)
			{
			dv->d[0]|=1;
			if (!XBN_usub(rem,rem,D)) goto end;
			}
/* CAN IMPROVE (and have now :=) */
		if (!XBN_rshift1(D,D)) goto end;
		}
	rem->neg=XBN_is_zero(rem)?0:m->neg;
	dv->neg=m->neg^d->neg;
	ret = 1;
 end:
	XBN_CTX_end(ctx);
	return(ret);
	}

#else

#if !defined(OPENSSL_NO_ASM) && !defined(OPENSSL_NO_INLINE_ASM) \
    && !defined(PEDANTIC) && !defined(XBN_DIV3W)
# if defined(__GNUC__) && __GNUC__>=2
#  if defined(__i386) || defined (__i386__)
   /*
    * There were two reasons for implementing this template:
    * - GNU C generates a call to a function (__udivdi3 to be exact)
    *   in reply to ((((XBN_ULLONG)n0)<<XBN_BITS2)|n1)/d0 (I fail to
    *   understand why...);
    * - divl doesn't only calculate quotient, but also leaves
    *   remainder in %edx which we can definitely use here:-)
    *
    *					<appro@fy.chalmers.se>
    */
#  define xbn_div_words(n0,n1,d0)		\
	({  asm volatile (			\
		"divl	%4"			\
		: "=a"(q), "=d"(rem)		\
		: "a"(n1), "d"(n0), "g"(d0)	\
		: "cc");			\
	    q;					\
	})
#  define REMAINDER_IS_ALREADY_CALCULATED
#  endif /* __<cpu> */
# endif /* __GNUC__ */
#endif /* OPENSSL_NO_ASM */


/* XBN_div computes  dv := num / divisor,  rounding towards zero, and sets up
 * rm  such that  dv*divisor + rm = num  holds.
 * Thus:
 *     dv->neg == num->neg ^ divisor->neg  (unless the result is zero)
 *     rm->neg == num->neg                 (unless the remainder is zero)
 * If 'dv' or 'rm' is NULL, the respective value is not returned.
 */
int XBN_div(XBIGNUM *dv, XBIGNUM *rm, const XBIGNUM *num, const XBIGNUM *divisor,
	   XBN_CTX *ctx)
	{
	int norm_shift,i,j,loop;
	XBIGNUM *tmp,wnum,*snum,*sdiv,*res;
	XBN_ULONG *resp,*wnump;
	XBN_ULONG d0,d1;
	int num_n,div_n;

	xbn_check_top(num);
	xbn_check_top(divisor);

	if (XBN_is_zero(divisor))
		{
		////BNerr(XBN_F_XBN_DIV,XBN_R_DIV_BY_ZERO);
		return(0);
		}

	if (XBN_ucmp(num,divisor) < 0)
		{
		if (rm != NULL)
			{ if (XBN_copy(rm,num) == NULL) return(0); }
		if (dv != NULL) XBN_zero(dv);
		return(1);
		}

	XBN_CTX_start(ctx);
	tmp=XBN_CTX_get(ctx);
	snum=XBN_CTX_get(ctx);
	sdiv=XBN_CTX_get(ctx);
	if (dv == NULL)
		res=XBN_CTX_get(ctx);
	else	res=dv;
	if (sdiv == NULL || res == NULL) goto err;
	tmp->neg=0;

	/* First we normalise the numbers */
	norm_shift=XBN_BITS2-((XBN_num_bits(divisor))%XBN_BITS2);
	if (!(XBN_lshift(sdiv,divisor,norm_shift))) goto err;
	sdiv->neg=0;
	norm_shift+=XBN_BITS2;
	if (!(XBN_lshift(snum,num,norm_shift))) goto err;
	snum->neg=0;
	div_n=sdiv->top;
	num_n=snum->top;
	loop=num_n-div_n;

	/* Lets setup a 'window' into snum
	 * This is the part that corresponds to the current
	 * 'area' being divided */
	XBN_init(&wnum);
	wnum.d=	 &(snum->d[loop]);
	wnum.top= div_n;
	wnum.dmax= snum->dmax+1; /* a bit of a lie */

	/* Get the top 2 words of sdiv */
	/* i=sdiv->top; */
	d0=sdiv->d[div_n-1];
	d1=(div_n == 1)?0:sdiv->d[div_n-2];

	/* pointer to the 'top' of snum */
	wnump= &(snum->d[num_n-1]);

	/* Setup to 'res' */
	res->neg= (num->neg^divisor->neg);
	if (!xbn_wexpand(res,(loop+1))) goto err;
	res->top=loop;
	resp= &(res->d[loop-1]);

	/* space for temp */
	if (!xbn_wexpand(tmp,(div_n+1))) goto err;

	if (XBN_ucmp(&wnum,sdiv) >= 0)
		{
		if (!XBN_usub(&wnum,&wnum,sdiv)) goto err;
		*resp=1;
		res->d[res->top-1]=1;
		}
	else
		res->top--;
	if (res->top == 0)
		res->neg = 0;
	resp--;

	for (i=0; i<loop-1; i++)
		{
		XBN_ULONG q,l0;
#if defined(XBN_DIV3W) && !defined(OPENSSL_NO_ASM)
		XBN_ULONG xbn_div_3_words(XBN_ULONG*,XBN_ULONG,XBN_ULONG);
		q=xbn_div_3_words(wnump,d1,d0);
#else
		XBN_ULONG n0,n1,rem=0;

		n0=wnump[0];
		n1=wnump[-1];
		if (n0 == d0)
			q=XBN_MASK2;
		else 			/* n0 < d0 */
			{
#ifdef XBN_LLONG
			XBN_ULLONG t2;

#if defined(XBN_LLONG) && defined(XBN_DIV2W) && !defined(xbn_div_words)
			q=(XBN_ULONG)(((((XBN_ULLONG)n0)<<XBN_BITS2)|n1)/d0);
#else
			q=xbn_div_words(n0,n1,d0);
#endif

#ifndef REMAINDER_IS_ALREADY_CALCULATED
			/*
			 * rem doesn't have to be XBN_ULLONG. The least we
			 * know it's less that d0, isn't it?
			 */
			rem=(n1-q*d0)&XBN_MASK2;
#endif
			t2=(XBN_ULLONG)d1*q;

			for (;;)
				{
				if (t2 <= ((((XBN_ULLONG)rem)<<XBN_BITS2)|wnump[-2]))
					break;
				q--;
				rem += d0;
				if (rem < d0) break; /* don't let rem overflow */
				t2 -= d1;
				}
#else /* !XBN_LLONG */
			XBN_ULONG t2l,t2h,ql,qh;

			q=xbn_div_words(n0,n1,d0);
#ifndef REMAINDER_IS_ALREADY_CALCULATED
			rem=(n1-q*d0)&XBN_MASK2;
#endif

#ifdef XBN_UMULT_HIGH
			t2l = d1 * q;
			t2h = XBN_UMULT_HIGH(d1,q);
#else
			t2l=LBITS(d1); t2h=HBITS(d1);
			ql =LBITS(q);  qh =HBITS(q);
			mul64(t2l,t2h,ql,qh); /* t2=(XBN_ULLONG)d1*q; */
#endif

			for (;;)
				{
				if ((t2h < rem) ||
					((t2h == rem) && (t2l <= wnump[-2])))
					break;
				q--;
				rem += d0;
				if (rem < d0) break; /* don't let rem overflow */
				if (t2l < d1) t2h--; t2l -= d1;
				}
#endif /* !XBN_LLONG */
			}
#endif /* !XBN_DIV3W */

		l0=xbn_mul_words(tmp->d,sdiv->d,div_n,q);
		wnum.d--; wnum.top++;
		tmp->d[div_n]=l0;
		for (j=div_n+1; j>0; j--)
			if (tmp->d[j-1]) break;
		tmp->top=j;

		j=wnum.top;
		if (!XBN_sub(&wnum,&wnum,tmp)) goto err;

		snum->top=snum->top+wnum.top-j;

		if (wnum.neg)
			{
			q--;
			j=wnum.top;
			if (!XBN_add(&wnum,&wnum,sdiv)) goto err;
			snum->top+=wnum.top-j;
			}
		*(resp--)=q;
		wnump--;
		}
	if (rm != NULL)
		{
		/* Keep a copy of the neg flag in num because if rm==num
		 * XBN_rshift() will overwrite it.
		 */
		int neg = num->neg;
		XBN_rshift(rm,snum,norm_shift);
		if (!XBN_is_zero(rm))
			rm->neg = neg;
		}
	XBN_CTX_end(ctx);
	return(1);
err:
	XBN_CTX_end(ctx);
	return(0);
	}

#endif
