/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2008,2009,2010 Sony Corporation, All Rights Reserved.
 *
 */

#ifndef NP_BASICTYPES__H
#define NP_BASICTYPES__H

/*===========================================================================*/
/** @file
 * @brief Basic type and macro definition (platform depended)
 *
 * @require none
 * @author  Norifumi Goto <n-goto@sm.sony.co.jp>
 */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*
 * type: fixed size integer
 */
#include "x_typedef.h"

#include "x_os.h"

/**
 * type: np_size_t
 */
typedef u_int32_t		np_size_t;

#ifdef __cplusplus
}
#endif

#endif /* NP_BASICTYPES__H */
