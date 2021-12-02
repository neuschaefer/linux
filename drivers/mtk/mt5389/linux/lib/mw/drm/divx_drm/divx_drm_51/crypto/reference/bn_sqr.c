/* crypto/bn/xbn_sqr.c */
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
//#include "cryptlib.h"
#include "bn_lcl.h"

#else

#include "bn_lcl.h"

#endif

/* r must not be a */
/* I've just gone over this and it is now %20 faster on x86 - eay - 27 Jun 96 */
int XBN_sqr(XBIGNUM *r, const XBIGNUM *a, XBN_CTX *ctx)
	{
	int max,al;
	int ret = 0;
	XBIGNUM *tmp,*rr;

#ifdef XBN_COUNT
	fprintf(stderr,"XBN_sqr %d * %d\n",a->top,a->top);
#endif
	xbn_check_top(a);

	al=a->top;
	if (al <= 0)
		{
		r->top=0;
		return(1);
		}

	XBN_CTX_start(ctx);
	rr=(a != r) ? r : XBN_CTX_get(ctx);
	tmp=XBN_CTX_get(ctx);
	if (tmp == NULL) goto err;

	max=(al+al);
	if (xbn_wexpand(rr,max+1) == NULL) goto err;

	if (al == 4)
		{
#ifndef XBN_SQR_COMBA
		XBN_ULONG t[8];
		xbn_sqr_normal(rr->d,a->d,4,t);
#else
		xbn_sqr_comba4(rr->d,a->d);
#endif
		}
	else if (al == 8)
		{
#ifndef XBN_SQR_COMBA
		XBN_ULONG t[16];
		xbn_sqr_normal(rr->d,a->d,8,t);
#else
		xbn_sqr_comba8(rr->d,a->d);
#endif
		}
	else 
		{
#if defined(XBN_RECURSION)
		if (al < XBN_SQR_RECURSIVE_SIZE_NORMAL)
			{
			XBN_ULONG t[XBN_SQR_RECURSIVE_SIZE_NORMAL*2];
			xbn_sqr_normal(rr->d,a->d,al,t);
			}
		else
			{
			int j,k;

			j=XBN_num_bits_word((XBN_ULONG)al);
			j=1<<(j-1);
			k=j+j;
			if (al == j)
				{
				if (xbn_wexpand(tmp,k*2) == NULL) goto err;
				xbn_sqr_recursive(rr->d,a->d,al,tmp->d);
				}
			else
				{
				if (xbn_wexpand(tmp,max) == NULL) goto err;
				xbn_sqr_normal(rr->d,a->d,al,tmp->d);
				}
			}
#else
		if (xbn_wexpand(tmp,max) == NULL) goto err;
		xbn_sqr_normal(rr->d,a->d,al,tmp->d);
#endif
		}

	rr->top=max;
	rr->neg=0;
	if ((max > 0) && (rr->d[max-1] == 0)) rr->top--;
	if (rr != r) XBN_copy(r,rr);
	ret = 1;
 err:
	XBN_CTX_end(ctx);
	return(ret);
	}

/* tmp must have 2*n words */
void xbn_sqr_normal(XBN_ULONG *r, const XBN_ULONG *a, int n, XBN_ULONG *tmp)
	{
	int i,j,max;
	const XBN_ULONG *ap;
	XBN_ULONG *rp;

	max=n*2;
	ap=a;
	rp=r;
	rp[0]=rp[max-1]=0;
	rp++;
	j=n;

	if (--j > 0)
		{
		ap++;
		rp[j]=xbn_mul_words(rp,ap,j,ap[-1]);
		rp+=2;
		}

	for (i=n-2; i>0; i--)
		{
		j--;
		ap++;
		rp[j]=xbn_mul_add_words(rp,ap,j,ap[-1]);
		rp+=2;
		}

	xbn_add_words(r,r,r,max);

	/* There will not be a carry */

	xbn_sqr_words(tmp,a,n);

	xbn_add_words(r,r,tmp,max);
	}

#ifdef XBN_RECURSION
/* r is 2*n words in size,
 * a and b are both n words in size.    (There's not actually a 'b' here ...)
 * n must be a power of 2.
 * We multiply and return the result.
 * t must be 2*n words in size
 * We calculate
 * a[0]*b[0]
 * a[0]*b[0]+a[1]*b[1]+(a[0]-a[1])*(b[1]-b[0])
 * a[1]*b[1]
 */
void xbn_sqr_recursive(XBN_ULONG *r, const XBN_ULONG *a, int n2, XBN_ULONG *t)
	{
	int n=n2/2;
	int zero,c1;
	XBN_ULONG ln,lo,*p;

#ifdef XBN_COUNT
	fprintf(stderr," xbn_sqr_recursive %d * %d\n",n2,n2);
#endif
	if (n2 == 4)
		{
#ifndef XBN_SQR_COMBA
		xbn_sqr_normal(r,a,4,t);
#else
		xbn_sqr_comba4(r,a);
#endif
		return;
		}
	else if (n2 == 8)
		{
#ifndef XBN_SQR_COMBA
		xbn_sqr_normal(r,a,8,t);
#else
		xbn_sqr_comba8(r,a);
#endif
		return;
		}
	if (n2 < XBN_SQR_RECURSIVE_SIZE_NORMAL)
		{
		xbn_sqr_normal(r,a,n2,t);
		return;
		}
	/* r=(a[0]-a[1])*(a[1]-a[0]) */
	c1=xbn_cmp_words(a,&(a[n]),n);
	zero=0;
	if (c1 > 0)
		xbn_sub_words(t,a,&(a[n]),n);
	else if (c1 < 0)
		xbn_sub_words(t,&(a[n]),a,n);
	else
		zero=1;

	/* The result will always be negative unless it is zero */
	p= &(t[n2*2]);

	if (!zero)
		xbn_sqr_recursive(&(t[n2]),t,n,p);
	else
		memset(&(t[n2]),0,n2*sizeof(XBN_ULONG));
	xbn_sqr_recursive(r,a,n,p);
	xbn_sqr_recursive(&(r[n2]),&(a[n]),n,p);

	/* t[32] holds (a[0]-a[1])*(a[1]-a[0]), it is negative or zero
	 * r[10] holds (a[0]*b[0])
	 * r[32] holds (b[1]*b[1])
	 */

	c1=(int)(xbn_add_words(t,r,&(r[n2]),n2));

	/* t[32] is negative */
	c1-=(int)(xbn_sub_words(&(t[n2]),t,&(t[n2]),n2));

	/* t[32] holds (a[0]-a[1])*(a[1]-a[0])+(a[0]*a[0])+(a[1]*a[1])
	 * r[10] holds (a[0]*a[0])
	 * r[32] holds (a[1]*a[1])
	 * c1 holds the carry bits
	 */
	c1+=(int)(xbn_add_words(&(r[n]),&(r[n]),&(t[n2]),n2));
	if (c1)
		{
		p= &(r[n+n2]);
		lo= *p;
		ln=(lo+c1)&XBN_MASK2;
		*p=ln;

		/* The overflow will stop before we over write
		 * words we should not overwrite */
		if (ln < (XBN_ULONG)c1)
			{
			do	{
				p++;
				lo= *p;
				ln=(lo+1)&XBN_MASK2;
				*p=ln;
				} while (ln == 0);
			}
		}
	}
#endif
