/*------------------------------------------------------------------------------

 Copyright 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/06/04
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/

#ifndef RE215_STDLIB_H
#define RE215_STDLIB_H

#include "re215_common.h"
#include "x_os.h"

/*
 <PORTING> Please modify if ANCI C standard library is not available.
*/

//#include <string.h>
#ifdef RE215_FP_ENABLE
#include <math.h>
#endif

// MTK
//extern void* x_memcpy(void*, const void*, unsigned int);
//extern void* x_memset(void*, uint8_t, unsigned int);
#define re215_memcpy  x_memcpy
#define re215_memset  x_memset

#define re215_log     log
#define re215_log10   log10

#endif /* RE215_STDLIB_H */
