/*------------------------------------------------------------------------------

 Copyright 2009, 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/03/19
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/

#ifndef RB211_STDLIB_H
#define RB211_STDLIB_H

#include "rb211_tuner_common.h"

/*
 <PORTING> Please modify if ANCI C standard library is not available.
*/

//#include <string.h>
#include "x_os.h"
#ifdef RB211_FP_ENABLE
#include <math.h>
#endif

#define rb211_memcpy  x_memcpy
#define rb211_memset  x_memset

#define rb211_log     log
#define rb211_log10   log10

#endif /* RB211_STDLIB_H */
