/*------------------------------------------------------------------------------

 Copyright 2009, 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/03/19
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/

#ifndef CXD2828_STDLIB_H
#define CXD2828_STDLIB_H

#include "cxd2828_common.h"

/*
 <PORTING> Please modify if ANCI C standard library is not available.
*/

//#include <string.h>
#include "x_os.h"
#ifdef CXD2828_FP_ENABLE
#include <math.h>
#endif

#define cxd2828_memcpy  x_memcpy
#define cxd2828_memset  x_memset

#define cxd2828_log     log
#define cxd2828_log10   log10

#endif /* CXD2828_STDLIB_H */
