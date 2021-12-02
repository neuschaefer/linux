/*
******************************************************************************
*
*   Copyright (C) 2002-2003, International Business Machines
*   Corporation and others.  All Rights Reserved.
*
******************************************************************************
*
* File uassert.h
*
*  Contains U_ASSERT macro
*
*    By default, U_ASSERT just wraps the C library assert macro.
*    By changing the definition here, the assert behavior for ICU can be changed
*    without affecting other non-ICU uses of the C library assert().
*
******************************************************************************
*/

#ifndef U_ASSERT_H
#define U_ASSERT_H

#include "x_dbg.h"

/* utypes.h is included to get the proper define for uint8_t */
#include "unicode/utypes.h"

#ifndef DBG_MOD_UC
#define DBG_MOD_UC      ((UINT32) 0x01800000)
#endif

#define U_ASSERT(exp)   DBG_ASSERT(exp, DBG_MOD_UC)
/* #define U_ASSERT(exp) DBG_ABORT(exp) */

#endif


