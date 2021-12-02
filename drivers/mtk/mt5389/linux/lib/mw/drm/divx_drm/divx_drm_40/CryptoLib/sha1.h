/* $Header: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_40/CryptoLib/sha1.h#1 $
// Copyright (c) 2003 DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivxNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.
*/

#ifndef SHA_1_H
#define SHA_1_H

#ifndef __KERNEL__

#include "divx_drm_adp_common.h"
#ifdef _MTK_BUILD_
#include "libc/stdlib.h"
#include "libc/memory.h"
#else
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>
#endif

#else

#include "../../divx_drm_adp_common.h"

#define malloc      x_mem_alloc
#define free        x_mem_free

#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
// sha1GetHash
//
// Gets the SHA1 hash string for
// the given buffer (should be at 
// least a few hundreds bytes long)
//
*/

char *sha1GetHash(unsigned char *buffer, 
				  unsigned int length, 
				  unsigned int *outlength);

#ifdef __cplusplus
};
#endif


#endif /* SHA_1_H */
