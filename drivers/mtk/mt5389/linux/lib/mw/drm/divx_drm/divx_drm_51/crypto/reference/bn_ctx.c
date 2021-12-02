/* crypto/bn/xbn_ctx.c */
/* Written by Ulf Moeller for the OpenSSL project. */
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

#ifndef XBN_CTX_DEBUG
# undef NDEBUG /* avoid conflicting definitions */
# define NDEBUG
#endif

#ifndef __KERNEL__

#ifdef _MTK_BUILD_
#include "libc/stdio.h"
#include "libc/stdlib.h"
#include "libc/memory.h"
#else
#include <stdio.h>
#include <assert.h>
#include <stdlib.h> //hmaxf malloc
#endif
//#include <memory.h>
//#include "cryptlib.h"
#include "bn_lcl.h"

#else

#include "bn_lcl.h"

#endif

XBN_CTX *XBN_CTX_new(void)
	{
	XBN_CTX *ret;

	//ret=(XBN_CTX *)OPENSSL_malloc(sizeof(XBN_CTX));
	ret=(XBN_CTX *)malloc(sizeof(XBN_CTX));
	if (ret == NULL)
		{
		//BNerr(XBN_F_XBN_CTX_NEW,ERR_R_MALLOC_FAILURE);//hmf .. no error
		return(NULL);
		}

	XBN_CTX_init(ret);
	ret->flags=XBN_FLG_MALLOCED;
	return(ret);
	}

void XBN_CTX_init(XBN_CTX *ctx)
	{
#if 0 /* explicit version */
	int i;
	ctx->tos = 0;
	ctx->flags = 0;
	ctx->depth = 0;
	ctx->too_many = 0;
	for (i = 0; i < XBN_CTX_NUM; i++)
		XBN_init(&(ctx->bn[i]));
#else
	memset(ctx, 0, sizeof *ctx);
#endif
	}

void XBN_CTX_free(XBN_CTX *ctx)
{
	int i;

	if (ctx == NULL) return;
	assert(ctx->depth == 0);

	for (i=0; i < XBN_CTX_NUM; i++)
		XBN_clear_free(&(ctx->bn[i]));
	if (ctx->flags & XBN_FLG_MALLOCED)
	{
		//OPENSSL_free(ctx);
		free(ctx);
	}
}

void XBN_CTX_start(XBN_CTX *ctx)
	{
	if (ctx->depth < XBN_CTX_NUM_POS)
		ctx->pos[ctx->depth] = ctx->tos;
	ctx->depth++;
	}


XBIGNUM *XBN_CTX_get(XBN_CTX *ctx)
	{
	/* Note: If XBN_CTX_get is ever changed to allocate XBIGNUMs dynamically,
	 * make sure that if XBN_CTX_get fails once it will return NULL again
	 * until XBN_CTX_end is called.  (This is so that callers have to check
	 * only the last return value.)
	 */
	if (ctx->depth > XBN_CTX_NUM_POS || ctx->tos >= XBN_CTX_NUM)
		{
		if (!ctx->too_many)
			{
			//BNerr(XBN_F_XBN_CTX_GET,XBN_R_TOO_MANY_TEMPORARY_VARIABLES);
			/* disable error code until XBN_CTX_end is called: */
			ctx->too_many = 1;
			}
		return NULL;
		}
	return (&(ctx->bn[ctx->tos++]));
	}

void XBN_CTX_end(XBN_CTX *ctx)
	{
	if (ctx == NULL) return;
	assert(ctx->depth > 0);
	if (ctx->depth == 0)
		/* should never happen, but we can tolerate it if not in
		 * debug mode (could be a 'goto err' in the calling function
		 * before XBN_CTX_start was reached) */
		XBN_CTX_start(ctx);

	ctx->too_many = 0;
	ctx->depth--;
	if (ctx->depth < XBN_CTX_NUM_POS)
		ctx->tos = ctx->pos[ctx->depth];
	}
