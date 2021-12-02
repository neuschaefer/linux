/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_40/CryptoLib/sha256.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

/* DivX modifications made to extract from libgcrypt. */

/* sha256.c - SHA256 hash function
 *	Copyright (C) 2003 Free Software Foundation, Inc.
 *
 * This file is part of Libgcrypt.
 *
 * Libgcrypt is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * Libgcrypt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef SHA256_H_INCLUDE
#define SHA256_H_INCLUDE

#ifndef __KERNEL__

#include "divx_drm_adp_common.h"
#ifdef _MTK_BUILD_
#include "libc/stdlib.h"
#else
#include <stdlib.h> /* size_t */
#endif

#else

#include "../../divx_drm_adp_common.h"

#include "x_common.h" /* size_t */

#endif

#define SHA256_BYTES 32

typedef struct {
  uint32_t  h0,h1,h2,h3,h4,h5,h6,h7;
  uint32_t  nblocks;
  uint8_t buf[64];
  int  count;
} SHA256_CONTEXT;

#ifdef __cplusplus
extern "C" {
#endif

void sha256_init(void *context);
void sha256_write(void *context, uint8_t *inbuf, size_t inlen);
void sha256_final(void *context);
uint8_t *sha256_read(void *context);

void sha256(uint8_t *data, uint32_t size, uint8_t *hash);

#ifdef __cplusplus
};
#endif

#endif
/* SHA256_H_INCLUDE */
